#include "sequencecreatorpane.h"
#include "widgets/dbitemlistcombobox.h"
#include "widgets/nameeditor.h"
#include "util/iconutil.h"
#include "util/strutil.h"
#include "metadata_loaders/sequence/sequenceinfoloader.h"
#include "connectivity/dbconnection.h"
#include <QtGui>

SequenceCreatorPane::SequenceCreatorPane(const QString &schemaName, const QString &objectName, QWidget *parent) :
    DbObjectCreatorPane(schemaName, objectName, parent), originalSequenceInfo(0)
{
    editMode=!objectName.isEmpty();

    QString placeholderText=tr("default");
    QRegExp digitsRegExp("\\d*", Qt::CaseSensitive, QRegExp::RegExp2);
    QRegExpValidator *intValidator=new QRegExpValidator(digitsRegExp, this);

    QVBoxLayout *mainLayout=new QVBoxLayout();
    QFormLayout *form=new QFormLayout();
    QWidget *formContainerWidget=new QWidget();
    formContainerWidget->setMinimumWidth(350);

    schemaList=new DbItemListComboBox(schemaName, "user", true);
    form->addRow(tr("Schema"), schemaList);

    sequenceNameEditor=new NameEditor();
    form->addRow(tr("Name"), sequenceNameEditor);

    minValueEditor=new QLineEdit();
    minValueEditor->setPlaceholderText(placeholderText);
    minValueEditor->setValidator(intValidator);
    form->addRow(tr("Minimum value"), minValueEditor);

    maxValueEditor=new QLineEdit();
    maxValueEditor->setPlaceholderText(placeholderText);
    maxValueEditor->setValidator(intValidator);
    form->addRow(tr("Maximum value"), maxValueEditor);

    incrementByEditor=new QLineEdit();
    incrementByEditor->setPlaceholderText(placeholderText);
    incrementByEditor->setValidator(intValidator);
    form->addRow(tr("Increment by"), incrementByEditor);

    cycleCheckbox=new QCheckBox();
    form->addRow(tr("Cycle"), cycleCheckbox);

    orderedCheckbox=new QCheckBox();
    form->addRow(tr("Ordered"), orderedCheckbox);

    cacheComboBox=new QComboBox();
    cacheComboBox->addItem("");
    cacheComboBox->addItem(tr("Enable"));
    cacheComboBox->addItem(tr("Disable"));
    form->addRow(tr("Cache"), cacheComboBox);

    cacheSizeEditor=new QLineEdit("20");
    cacheSizeEditor->setValidator(intValidator);
    form->addRow(tr("Cache size"), cacheSizeEditor);

    startWithEditor=new QLineEdit();
    startWithEditor->setPlaceholderText(placeholderText);
    startWithEditor->setValidator(intValidator);
    form->addRow(editMode ? tr("Current value") : tr("Start with"), startWithEditor);

    formContainerWidget->setLayout(form);

    QScrollArea *scrollArea=new QScrollArea();
    scrollArea->setContentsMargins(0,0,0,0);
    scrollArea->setWidget(formContainerWidget);
    //scrollArea->setWidgetResizable(true);

    mainLayout->addWidget(scrollArea);
    mainLayout->setContentsMargins(0,0,0,0);
    setLayout(mainLayout);

    if(editMode){
        disableControlsForEditMode();
    }

    enableControls();
    connect(cacheComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(enableControls()));

    connect(schemaList, SIGNAL(currentIndexChanged(int)), this, SIGNAL(ddlChanged()));
    connect(sequenceNameEditor, SIGNAL(editingFinished()), this, SIGNAL(ddlChanged()));
    connect(minValueEditor, SIGNAL(editingFinished()), this, SIGNAL(ddlChanged()));
    connect(maxValueEditor, SIGNAL(editingFinished()), this, SIGNAL(ddlChanged()));
    connect(incrementByEditor, SIGNAL(editingFinished()), this, SIGNAL(ddlChanged()));
    connect(cycleCheckbox, SIGNAL(stateChanged(int)), this, SIGNAL(ddlChanged()));
    connect(orderedCheckbox, SIGNAL(stateChanged(int)), this, SIGNAL(ddlChanged()));
    connect(cacheComboBox, SIGNAL(currentIndexChanged(int)), this, SIGNAL(ddlChanged()));
    connect(cacheSizeEditor, SIGNAL(editingFinished()), this, SIGNAL(ddlChanged()));
    connect(startWithEditor, SIGNAL(editingFinished()), this, SIGNAL(ddlChanged()));
}

SequenceCreatorPane::~SequenceCreatorPane()
{
    delete originalSequenceInfo;
}

void SequenceCreatorPane::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    DbObjectCreatorPane::setQueryScheduler(queryScheduler);

    if(queryScheduler->getDb()->getSchemaName()!=schemaList->currentText()){
        sequenceNameEditor->setEnabled(false);
    }

    if(editMode){
        schemaList->addItem(IconUtil::getIcon("user"), schemaList->currentText());
        schemaList->setCurrentIndex(0);

        SequenceInfoLoader *metadataLoader=new SequenceInfoLoader(this->queryScheduler, this->schemaName, this->objectName, this);
        connect(metadataLoader, SIGNAL(objectInfoReady(DbObjectInfo*,MetadataLoader*)), this, SLOT(sequenceInfoReady(DbObjectInfo*,MetadataLoader*)));
        connect(metadataLoader, SIGNAL(loadError(QString,OciException,MetadataLoader*)), this, SLOT(loadError(QString,OciException,MetadataLoader*)));
        metadataLoader->loadObjectInfo();
    }else{
        schemaList->loadItems(this->queryScheduler, "get_schema_list");
        emit objectInfoLoaded();
    }
}

QString SequenceCreatorPane::generateCreateDdl()
{
    return getSequenceInfo().generateDdl(SequenceCreateDdlOptions());
}

QList<QueryListItem> SequenceCreatorPane::generateAlterDdl()
{
    QList<QueryListItem> result;

    result.append(QueryListItem(this, getSequenceInfo().generateDiffDdl(*originalSequenceInfo, SequenceDiffDdlOptions())));

    return result;
}

bool SequenceCreatorPane::beforeAlter()
{
    Q_ASSERT(editMode);

    if(getSequenceInfo().needsRecreation(*originalSequenceInfo)){
        return QMessageBox::question(this->window(), tr("Drop and recreate"),
                                     tr("Sequence needs to be dropped and recreated in order to set new current value.\nDo you want to proceed?"),
                                     QMessageBox::Ok | QMessageBox::Cancel)==QMessageBox::Ok;
    }

    return true;
}

QString SequenceCreatorPane::getSchemaName() const
{
    return schemaList->lineEdit()->text().trimmed().toUpper();
}

QString SequenceCreatorPane::getObjectName() const
{
    return sequenceNameEditor->text().trimmed().toUpper();
}

void SequenceCreatorPane::alterQuerySucceeded(const QString &taskName)
{
    Q_ASSERT(originalSequenceInfo);

    if(taskName=="drop_sequence"){
        originalSequenceInfo->dropped=true;
    }else if(taskName=="create_sequence"){
        *originalSequenceInfo=getSequenceInfo();
    }else if(taskName=="rename_sequence"){
        originalSequenceInfo->name=sequenceNameEditor->text().trimmed().toUpper();
    }else if(taskName=="alter_sequence"){
        *originalSequenceInfo=getSequenceInfo();
    }
}

void SequenceCreatorPane::enableControls()
{
    cacheSizeEditor->setEnabled(cacheComboBox->currentIndex()==1);
}

void SequenceCreatorPane::sequenceInfoReady(DbObjectInfo *objectInfo, MetadataLoader *loader)
{
    Q_ASSERT(originalSequenceInfo==0);
    originalSequenceInfo=static_cast<SequenceInfo*>(objectInfo);

    setSequenceInfo(originalSequenceInfo);

    loader->deleteLater();

    emit objectInfoLoaded();
}

void SequenceCreatorPane::loadError(const QString &taskName, const OciException &ex, MetadataLoader *loader)
{
    QMessageBox::critical(this, tr("Failed to load sequence information"),
                          tr("Task name: %1\nError: %2").arg(taskName, ex.getErrorMessage()));

    loader->deleteLater();
}

SequenceInfo SequenceCreatorPane::getSequenceInfo() const
{
    SequenceInfo info;

    info.owner=schemaList->lineEdit()->text().trimmed().toUpper();
    info.name=sequenceNameEditor->text().trimmed().toUpper();
    info.minValue=minValueEditor->text();
    info.maxValue=maxValueEditor->text();
    info.incrementBy=incrementByEditor->text();
    info.cycle=cycleCheckbox->isChecked();
    info.ordered=orderedCheckbox->isChecked();

    int cacheIx = cacheComboBox->currentIndex();
    if(cacheIx==0){
        info.cacheSize= editMode ? "0" : "";
    }else if(cacheIx==1){
        info.cacheSize=cacheSizeEditor->text();
    }else{
        info.cacheSize="0";
    }

    info.lastNumber=startWithEditor->text();

    return info;
}

void SequenceCreatorPane::setSequenceInfo(SequenceInfo *sequenceInfo)
{
    sequenceNameEditor->setText(sequenceInfo->name);
    minValueEditor->setText(sequenceInfo->minValue);
    maxValueEditor->setText(sequenceInfo->maxValue);
    incrementByEditor->setText(sequenceInfo->incrementBy);
    cycleCheckbox->setChecked(sequenceInfo->cycle);
    orderedCheckbox->setChecked(sequenceInfo->ordered);
    cacheComboBox->setCurrentIndex(sequenceInfo->cacheSize=="0" ? 2 : 1);
    cacheSizeEditor->setText(sequenceInfo->cacheSize);
    startWithEditor->setText(sequenceInfo->lastNumber);
}

void SequenceCreatorPane::disableControlsForEditMode()
{
    schemaList->setEnabled(false);
}
