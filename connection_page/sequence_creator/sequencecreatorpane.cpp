#include "sequencecreatorpane.h"
#include "widgets/dbitemlistcombobox.h"
#include "widgets/nameeditor.h"
#include "util/strutil.h"
#include "beans/sequenceinfo.h"
#include "interfaces/iqueryscheduler.h"
#include "connectivity/dbconnection.h"
#include "connection_page/db_object_creator/dbobjectcreator.h"
#include <QtGui>

SequenceCreatorPane::SequenceCreatorPane(DbObjectCreator *objectCreator, QWidget *parent) :
    DbObjectCreatorSimplePane(objectCreator, parent)
{

}

void SequenceCreatorPane::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    DbObjectCreatorSimplePane::setQueryScheduler(queryScheduler);

    if(queryScheduler->getDb()->getSchemaName()!=schemaList->currentText()){
        sequenceNameEditor->setEnabled(false);
    }
}

QLayout *SequenceCreatorPane::createForm()
{
    QRegExp digitsRegExp("\\d*", Qt::CaseSensitive, QRegExp::RegExp2);
    QRegExpValidator *intValidator=new QRegExpValidator(digitsRegExp, this);

    QFormLayout *form=new QFormLayout();

    schemaList=new DbItemListComboBox(objectCreator->getOriginalSchemaName(), "user", true);
    form->addRow(tr("Schema"), schemaList);

    sequenceNameEditor=new NameEditor();
    form->addRow(tr("Name"), sequenceNameEditor);

    minValueEditor=new QLineEdit();
    minValueEditor->setValidator(intValidator);
    form->addRow(tr("Minimum value"), minValueEditor);

    maxValueEditor=new QLineEdit();
    maxValueEditor->setValidator(intValidator);
    form->addRow(tr("Maximum value"), maxValueEditor);

    incrementByEditor=new QLineEdit();
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
    startWithEditor->setValidator(intValidator);
    form->addRow((getCreatorMode() == DbObjectCreator::EditExisting) ? tr("Current value") : tr("Start with"), startWithEditor);

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

    if(getCreatorMode() != DbObjectCreator::EditExisting){
        minValueEditor->setPlaceholderText(PLACEHOLDER_DEFAULT);
        maxValueEditor->setPlaceholderText(PLACEHOLDER_DEFAULT);
        incrementByEditor->setPlaceholderText(PLACEHOLDER_DEFAULT);
        startWithEditor->setPlaceholderText(PLACEHOLDER_DEFAULT);
    }

    return form;
}

QString SequenceCreatorPane::getSchemaName() const
{
    return schemaList->lineEdit()->text().trimmed().toUpper();
}

QString SequenceCreatorPane::getObjectName() const
{
    return sequenceNameEditor->text().trimmed().toUpper();
}

bool SequenceCreatorPane::beforeAlter() const
{
    Q_ASSERT(getCreatorMode() == DbObjectCreator::EditExisting);

    SequenceInfo *originalSequenceInfo=getOriginalObjectInfo<SequenceInfo>();
    if(getSequenceInfo().needsRecreation(*originalSequenceInfo)){
        return QMessageBox::question(this->window(), tr("Drop and recreate"),
                                     tr("Sequence needs to be dropped and recreated in order to set new current value.\nDo you want to proceed?"),
                                     QMessageBox::Ok | QMessageBox::Cancel)==QMessageBox::Ok;
    }

    return true;
}

QString SequenceCreatorPane::generateCreateDdl()
{
    return getSequenceInfo().generateDdl(SequenceCreateDdlOptions());
}

QList<QueryListItem> SequenceCreatorPane::generateAlterDdl()
{
    QList<QueryListItem> result;

    SequenceInfo *originalSequenceInfo=getOriginalObjectInfo<SequenceInfo>();
    result.append(QueryListItem(this, getSequenceInfo().generateDiffDdl(*originalSequenceInfo, SequenceDiffDdlOptions())));

    return result;
}

void SequenceCreatorPane::alterQuerySucceeded(const QString &taskName)
{
    SequenceInfo *originalSequenceInfo=getOriginalObjectInfo<SequenceInfo>();
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
        info.cacheSize= (getCreatorMode() == DbObjectCreator::EditExisting) ? "0" : "";
    }else if(cacheIx==1){
        info.cacheSize=cacheSizeEditor->text();
    }else{
        info.cacheSize="0";
    }

    info.lastNumber=startWithEditor->text();

    return info;
}

void SequenceCreatorPane::setObjectInfo(DbObjectInfo *objectInfo)
{
    SequenceInfo *sequenceInfo=static_cast<SequenceInfo*>(objectInfo);
    Q_ASSERT(sequenceInfo);

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
