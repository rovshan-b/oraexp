#include "synonymcreatorpane.h"
#include "widgets/dbitemlistcombobox.h"
#include "widgets/nameeditor.h"
#include "widgets/lineeditwithbutton.h"
#include "util/strutil.h"
#include "util/widgethelper.h"
#include "util/queryutil.h"
#include "beans/synonyminfo.h"
#include "interfaces/iqueryscheduler.h"
#include "connectivity/dbconnection.h"
#include "connection_page/db_object_creator/dbobjectcreator.h"
#include "dialogs/genericresultsetviewerdialog.h"
#include <QtGui>

SynonymCreatorPane::SynonymCreatorPane(DbObjectCreator *objectCreator, QWidget *parent) :
    DbObjectCreatorSimplePane(objectCreator, parent)
{

}

void SynonymCreatorPane::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    DbObjectCreatorSimplePane::setQueryScheduler(queryScheduler);

    QString currentDbSchema = queryScheduler->getDb()->getSchemaName();
    WidgetHelper::setComboBoxText(schemaList, currentDbSchema);
    WidgetHelper::setComboBoxText(ownerComboBox, currentDbSchema);
    dbLinkComboBox->loadItems(queryScheduler, "get_dblink_list_plain", QList<Param*>() << new Param("owner", currentDbSchema));
    ownerComboBox->loadItems(queryScheduler, "get_schema_list");
}

QLayout *SynonymCreatorPane::createForm()
{
    QVBoxLayout *mainLayout=new QVBoxLayout();

    QGroupBox *synonymInfoBox=new QGroupBox(tr("Synonym"));
    QFormLayout *form1=new QFormLayout();

    publicCheckBox=new QCheckBox();
    publicCheckBox->setChecked(!objectCreator->propertyValue("public").isEmpty());
    form1->addRow(tr("Public"), publicCheckBox);

    schemaList=new DbItemListComboBox(objectCreator->getOriginalSchemaName(), "user", true, false);
    form1->addRow(tr("Schema"), schemaList);

    synonymNameEditor=new NameEditor();
    form1->addRow(tr("Synonym name     "), synonymNameEditor);

    synonymInfoBox->setLayout(form1);
    mainLayout->addWidget(synonymInfoBox);

    QGroupBox *targetObjectInfoBox=new QGroupBox(tr("Target object"));
    QFormLayout *form2=new QFormLayout();

    isOverDbLinkCheckBox=new QCheckBox();
    form2->addRow(tr("Over database link"), isOverDbLinkCheckBox);

    dbLinkComboBox=new DbItemListComboBox("", "dblink", true);
    form2->addRow(tr("Database link"), dbLinkComboBox);

    ownerComboBox=new DbItemListComboBox("", "user", true, false);
    form2->addRow(tr("Owner"), ownerComboBox);

    objectNameEditor=new LineEditWithButton(this);
    form2->addRow(tr("Object"), objectNameEditor);

    targetObjectInfoBox->setLayout(form2);
    mainLayout->addWidget(targetObjectInfoBox);

    enableControls();

    connect(publicCheckBox, SIGNAL(stateChanged(int)), this, SLOT(enableControls()));
    connect(isOverDbLinkCheckBox, SIGNAL(stateChanged(int)), this, SLOT(enableControls()));
    connect(objectNameEditor, SIGNAL(buttonClicked(LineEditWithButton*)), this, SLOT(showTargetObjectList()));

    connect(publicCheckBox, SIGNAL(stateChanged(int)), this, SIGNAL(ddlChanged()));
    connect(schemaList->lineEdit(), SIGNAL(editingFinished()), this, SIGNAL(ddlChanged()));
    connect(synonymNameEditor, SIGNAL(editingFinished()), this, SIGNAL(ddlChanged()));
    connect(isOverDbLinkCheckBox, SIGNAL(stateChanged(int)), this, SIGNAL(ddlChanged()));
    connect(dbLinkComboBox->lineEdit(), SIGNAL(editingFinished()), this, SIGNAL(ddlChanged()));
    connect(ownerComboBox->lineEdit(), SIGNAL(editingFinished()), this, SIGNAL(ddlChanged()));
    connect(objectNameEditor->lineEdit(), SIGNAL(editingFinished()), this, SIGNAL(ddlChanged()));

    //connect(isOverDbLinkCheckBox, SIGNAL(stateChanged(int)), this, SLOT(loadTargetObjectList()));
    //connect(dbLinkComboBox->lineEdit(), SIGNAL(editingFinished()), this, SLOT(loadTargetObjectList()));
    //connect(ownerComboBox->lineEdit(), SIGNAL(editingFinished()), this, SLOT(loadTargetObjectList()));

    return mainLayout;
}

QString SynonymCreatorPane::getSchemaName() const
{
    return schemaList->lineEdit()->text().trimmed().toUpper();
}

QString SynonymCreatorPane::getObjectName() const
{
    return synonymNameEditor->text().trimmed().toUpper();
}

QString SynonymCreatorPane::generateCreateDdl()
{
    return getSynonymInfo().generateDdl(false);
}

QList<QueryListItem> SynonymCreatorPane::generateAlterDdl()
{
    QList<QueryListItem> result;

    SynonymInfo *originalSynonymInfo=getOriginalObjectInfo<SynonymInfo>();
    result.append(QueryListItem(this, getSynonymInfo().generateDiffDdl(*originalSynonymInfo)));

    return result;
}

bool SynonymCreatorPane::beforeCreate() const
{
    SynonymInfo info=getSynonymInfo();
    return WidgetHelper::validate(&info, this->window());
}

bool SynonymCreatorPane::beforeAlter() const
{
    return beforeCreate();
}

void SynonymCreatorPane::alterQuerySucceeded(const QString &taskName)
{
    SynonymInfo *originalSynonymInfo=getOriginalObjectInfo<SynonymInfo>();
    Q_ASSERT(originalSynonymInfo);

    if(taskName=="drop_synonym"){
        originalSynonymInfo->dropped=true;
    }else if(taskName=="create_synonym"){
        *originalSynonymInfo=getSynonymInfo();
    }else if(taskName=="rename_synonym"){
        originalSynonymInfo->name=synonymNameEditor->text().trimmed().toUpper();
    }else if(taskName=="recreate_synonym"){
        *originalSynonymInfo=getSynonymInfo();
    }
}

SynonymInfo SynonymCreatorPane::getSynonymInfo() const
{
    SynonymInfo info;

    info.owner=publicCheckBox->isChecked() ? "PUBLIC" : schemaList->lineEdit()->text().trimmed().toUpper();
    info.name=synonymNameEditor->text().trimmed().toUpper();
    info.overDbLink=isOverDbLinkCheckBox->isChecked();
    info.dbLinkName=dbLinkComboBox->lineEdit()->text().trimmed().toUpper();
    info.targetSchema=ownerComboBox->lineEdit()->text().trimmed().toUpper();
    info.targetObject=objectNameEditor->lineEdit()->text().trimmed().toUpper();

    return info;
}

void SynonymCreatorPane::setObjectInfo(DbObjectInfo *objectInfo)
{
    SynonymInfo *info=static_cast<SynonymInfo*>(objectInfo);
    Q_ASSERT(info);

    publicCheckBox->setChecked(info->owner=="PUBLIC");
    WidgetHelper::setComboBoxText(schemaList, info->owner);
    synonymNameEditor->setText(info->name);
    isOverDbLinkCheckBox->setChecked(!info->dbLinkName.isEmpty());
    WidgetHelper::setComboBoxText(dbLinkComboBox, info->dbLinkName);
    WidgetHelper::setComboBoxText(ownerComboBox, info->targetSchema);
    objectNameEditor->lineEdit()->setText(info->targetObject);


    QString currentDbSchema = queryScheduler->getDb()->getSchemaName();
    if(editMode && currentDbSchema!=schemaList->currentText() && !publicCheckBox->isChecked()){
        synonymNameEditor->setEnabled(false);
    }
}

void SynonymCreatorPane::enableControls()
{
    schemaList->setEnabled(!publicCheckBox->isChecked() && !editMode);
    dbLinkComboBox->setEnabled(isOverDbLinkCheckBox->isChecked());
}

void SynonymCreatorPane::showTargetObjectList()
{
    Q_ASSERT(this->queryScheduler);

    QString dbLinkName = isOverDbLinkCheckBox->isChecked() ? dbLinkComboBox->lineEdit()->text().trimmed().toUpper() : "";
    QString owner = ownerComboBox->lineEdit()->text().trimmed().toUpper();
    if((isOverDbLinkCheckBox->isChecked() && dbLinkName.isEmpty()) || owner.isEmpty()){
        return;
    }

    GenericResultsetViewerDialog dialog(this->queryScheduler,
                                        QueryUtil::getQuery("get_object_list_for_synonym", this->queryScheduler->getDb()),
                                        QList<Param*>() << new Param("owner", owner),
                                        dbLinkName,
                                        this->window(),
                                        qMakePair<QString,QString>("OBJECT_NAME", "ICON_NAME"));
    dialog.setWindowTitle(tr("Select target object"));
    if(dialog.exec()){
        objectNameEditor->lineEdit()->setText(dialog.selectedText);
        emit ddlChanged();
    }
}

void SynonymCreatorPane::disableControlsForEditMode()
{
    publicCheckBox->setEnabled(false);
    schemaList->setEnabled(false);
}
