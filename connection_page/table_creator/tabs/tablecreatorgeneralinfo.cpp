#include "tablecreatorgeneralinfo.h"
#include "widgets/dbitemlistcombobox.h"
#include "widgets/nameeditor.h"
#include "widgets/storageparamswidget.h"
#include "widgets/lineeditwithbutton.h"
#include "widgets/lineeditwithautocompleter.h"
#include "dialogs/storageparamsdialog.h"
#include "dialogs/tableadditionalattributesdialog.h"
#include "dialogs/indexorganizedtablepropertiesdialog.h"
#include "util/iconutil.h"
#include "util/dialoghelper.h"
#include "connectivity/dbconnection.h"
#include "../tablecreatortabs.h"
#include "util/dbutil.h"
#include <QtGui>

TableCreatorGeneralInfo::TableCreatorGeneralInfo(const QString &schemaName,
                                                 const QString &tableName,
                                                 TableCreatorTabs *tableCreator,
                                                 bool editMode,
                                                 QWidget *parent) :
    TableCreatorTab(tableCreator, editMode, parent), originalGeneralInfo(0)
{
    QVBoxLayout *layout=new QVBoxLayout();

    QFormLayout *form=new QFormLayout();

    tableTypeComboBox=new QComboBox();
    QIcon tmpTableIcon=IconUtil::getIcon("table_temporary");
    tableTypeComboBox->addItem(IconUtil::getIcon("table"), tr("Heap (Default)"));
    tableTypeComboBox->addItem(tmpTableIcon, tr("Temporary - transaction"));
    tableTypeComboBox->addItem(tmpTableIcon, tr("Temporary - session"));
    tableTypeComboBox->addItem(IconUtil::getIcon("table_index_organized"), tr("Index organized"));
    tableTypeComboBox->addItem(IconUtil::getIcon("table_external"), tr("External"));
    tableTypeComboBox->setCurrentIndex(0);
    form->addRow(tr("Type"), tableTypeComboBox);

    schemaList=new DbItemListComboBox(schemaName, "user", true, true);
    form->addRow(tr("Schema"), schemaList);

    tableNameEditor=new NameEditor();
    tableNameEditor->setText(tableName);
    form->addRow(tr("Name"), tableNameEditor);

    tableCommentEditor=new QPlainTextEdit();
    tableCommentEditor->setMaximumHeight(80);
    tableCommentEditor->setTabChangesFocus(true);
    form->addRow(tr("Comments"), tableCommentEditor);

    storageParamsEditor=new LineEditWithButton(this);
    storageParamsEditor->lineEdit()->setReadOnly(true);
    form->addRow(tr("Physical attributes"), storageParamsEditor);

    additionalAttributesEditor=new LineEditWithButton(this);
    additionalAttributesEditor->lineEdit()->setReadOnly(true);
    form->addRow(tr("Properties"), additionalAttributesEditor);

    indexOrganizedTablePropertiesEditor=new LineEditWithButton(this);
    indexOrganizedTablePropertiesEditor->lineEdit()->setReadOnly(true);
    form->addRow(tr("IOT properties"), indexOrganizedTablePropertiesEditor);
    indexOrganizedTablePropertiesEditor->setEnabled(false);

    layout->addLayout(form);

    layout->addStretch(1);

    setLayout(layout);

    connect(schemaList->lineEdit(), SIGNAL(editingFinished()), this, SIGNAL(ddlChanged()));
    connect(schemaList->lineEdit(), SIGNAL(editingFinished()), this, SIGNAL(currentSchemaChanged()));
    connect(tableNameEditor, SIGNAL(editingFinished()), this, SIGNAL(ddlChanged()));
    tableCommentEditor->installEventFilter(this);

    connect(storageParamsEditor, SIGNAL(buttonClicked(LineEditWithButton*)), this, SLOT(storageParamsButtonClicked(LineEditWithButton*)));
    connect(additionalAttributesEditor, SIGNAL(buttonClicked(LineEditWithButton*)), this, SLOT(additionalAttributesButtonClicked(LineEditWithButton*)));
    connect(indexOrganizedTablePropertiesEditor, SIGNAL(buttonClicked(LineEditWithButton*)), this, SLOT(indexOrganizedPropertiesButtonClicked(LineEditWithButton*)));
    connect(tableTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(tableTypeComboBoxChanged(int)));


    if(editMode){
        disableControlsForEditMode();
    }
}

void TableCreatorGeneralInfo::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    TableCreatorTab::setQueryScheduler(queryScheduler);

    if(!isEditMode()){
        schemaList->loadItems(this->queryScheduler, "get_schema_list");
    }else{
        schemaList->addItem(IconUtil::getIcon("user"), schemaList->currentText());
        schemaList->setCurrentIndex(0);
    }

    //tableNameEditor->setConnection(db);

    //if(isEditMode()){
    //    loadTableInfo();
    //}
}

void TableCreatorGeneralInfo::disableControlsForEditMode()
{
    tableTypeComboBox->setEnabled(false);
    schemaList->setEnabled(false);
}

void TableCreatorGeneralInfo::setTableInfo(TableInfo *tableInfo)
{
    originalGeneralInfo=&tableInfo->generalInfo;

    tableCommentEditor->setPlainText(tableInfo->generalInfo.comments);

    storageParams = tableInfo->generalInfo.storageParams;
    additionalAttributes = tableInfo->generalInfo.additionalAttributes;
    indexOrganizedProperties = tableInfo->generalInfo.indexOrganizedProperties;

    tableTypeComboBox->setCurrentIndex((int)tableInfo->generalInfo.tableType);
}

void TableCreatorGeneralInfo::setColumnListRetriever(IStringListRetriever *columnListRetriever)
{
    this->columnListRetriever=columnListRetriever;
}

bool TableCreatorGeneralInfo::eventFilter(QObject *obj, QEvent *event)
{
    if (obj==tableCommentEditor && event->type() == QEvent::FocusOut) {
        emit ddlChanged();
    }

    // standard event processing
    return TableCreatorTab::eventFilter(obj, event);
}

void TableCreatorGeneralInfo::storageParamsButtonClicked(LineEditWithButton *lineEditWithButton)
{
    if(queryScheduler==0){
        return;
    }

    if(DialogHelper::showStorageParamsDialog(this->window(), queryScheduler, isEditMode(), true, this->storageParams)){
        lineEditWithButton->lineEdit()->setText(
                    isEditMode()
                      ? storageParams.generateDiffDdl(originalGeneralInfo->storageParams)
                      : storageParams.generateDdl().trimmed()
                    );
        lineEditWithButton->lineEdit()->setCursorPosition(0);

        emit ddlChanged();
    }
}

void TableCreatorGeneralInfo::additionalAttributesButtonClicked(LineEditWithButton *lineEditWithButton)
{
    if(this->queryScheduler==0){
        return;
    }

    TableAdditionalAttributesDialog dialog(this->queryScheduler, getTableType(), isEditMode(), this->window());
    dialog.setAttributes(additionalAttributes);

    if(dialog.exec()){
        additionalAttributes=dialog.getAttributes();
        lineEditWithButton->lineEdit()->setText(
                    isEditMode()
                      ? additionalAttributes.generateDiffDdl(originalGeneralInfo->additionalAttributes)
                      : additionalAttributes.generateDdl(getTableType(), true).trimmed()
                    );
        lineEditWithButton->lineEdit()->setCursorPosition(0);

        emit ddlChanged();
    }
}

void TableCreatorGeneralInfo::indexOrganizedPropertiesButtonClicked(LineEditWithButton *lineEditWithButton)
{
    if(this->queryScheduler==0){
        return;
    }

    IndexOrganizedTablePropertiesDialog dialog(this->queryScheduler, this->columnListRetriever, isEditMode(), this->window());
    dialog.setAttributes(indexOrganizedProperties, isEditMode() ? originalGeneralInfo->indexOrganizedProperties : IndexOrganizedTableProperties());

    if(dialog.exec()){
        indexOrganizedProperties=dialog.getAttributes();
        lineEditWithButton->lineEdit()->setText(
                    isEditMode()
                      ? indexOrganizedProperties.generateDiffDdl(originalGeneralInfo->indexOrganizedProperties)
                      : indexOrganizedProperties.generateDdl().trimmed()
                    );
        lineEditWithButton->lineEdit()->setCursorPosition(0);

        emit ddlChanged();
    }
}

QString TableCreatorGeneralInfo::getSchema() const
{
    return schemaList->lineEdit()->text().trimmed().toUpper();
}

QString TableCreatorGeneralInfo::getTableName() const
{
    return tableNameEditor->text().trimmed().toUpper();
}

void TableCreatorGeneralInfo::tableTypeComboBoxChanged(int newTableType)
{
    OraExp::TableType tableType=(OraExp::TableType)newTableType;
    bool enableStorageOptions=!(tableType==OraExp::TableTypeTemporaryTransaction ||
                               tableType==OraExp::TableTypeTemporarySession ||
                               tableType==OraExp::TableTypeExternal);
    storageParamsEditor->setEnabled(enableStorageOptions);
    additionalAttributesEditor->setEnabled(enableStorageOptions);

    bool enableComments=(tableType!=OraExp::TableTypeExternal);

    tableCommentEditor->setEnabled(enableComments);

    bool enableIndexOrganizedPropertiesWidget=(tableType==OraExp::TableTypeIndexOrganized);

    indexOrganizedTablePropertiesEditor->setEnabled(enableIndexOrganizedPropertiesWidget);

    //regenerate additional attributes ddl, because it might change depending on table type
    additionalAttributesEditor->lineEdit()->setText(isEditMode() ?
                                                        additionalAttributes.generateDiffDdl(originalGeneralInfo->additionalAttributes) :
                                                        additionalAttributes.generateDdl(getTableType(), true));
    additionalAttributesEditor->lineEdit()->setCursorPosition(0);

    emit tableTypeChanged(tableType);
}

OraExp::TableType TableCreatorGeneralInfo::getTableType() const
{
    return (OraExp::TableType)tableTypeComboBox->currentIndex();
}

TableGeneralInfo TableCreatorGeneralInfo::getTableGeneralInfo() const
{
    TableGeneralInfo info;

    info.schema=schemaList->lineEdit()->text().trimmed().toUpper();
    info.tableName=tableNameEditor->text().trimmed().toUpper();
    info.tableType=getTableType();
    info.storageParams=this->storageParams;
    info.additionalAttributes=this->additionalAttributes;
    info.indexOrganizedProperties=this->indexOrganizedProperties;
    info.comments=tableCommentEditor->toPlainText().trimmed();

    return info;
}

void TableCreatorGeneralInfo::alterQuerySucceeded(const QString &taskName)
{
    Q_ASSERT(originalGeneralInfo);

    if(taskName=="rename_table"){
        originalGeneralInfo->tableName=tableNameEditor->text().trimmed().toUpper();
        objectCreator->setOriginalTableName(tableNameEditor->text().trimmed().toUpper());
    }else if(taskName=="alter_table_comments"){
        originalGeneralInfo->comments=tableCommentEditor->toPlainText().trimmed();
    }else if(taskName=="alter_table_additional_attributes"){
        originalGeneralInfo->additionalAttributes = additionalAttributes;
        additionalAttributesEditor->lineEdit()->setText("");
    }else if(taskName=="alter_table_storage"){
        originalGeneralInfo->storageParams = storageParams;
        storageParamsEditor->lineEdit()->setText("");
    }else if(taskName=="alter_iot_attributes"){
        originalGeneralInfo->indexOrganizedProperties = indexOrganizedProperties;
        indexOrganizedTablePropertiesEditor->lineEdit()->setText("");
    }
}
