#include "sequencecreatorpane.h"
#include "widgets/dbitemlistcombobox.h"
#include "widgets/nameeditor.h"
#include "util/iconutil.h"
#include "util/strutil.h"
#include <QtGui>

SequenceCreatorPane::SequenceCreatorPane(const QString &schemaName, const QString &objectName, QWidget *parent) :
    DbObjectCreatorPane(schemaName, objectName, parent)
{
    editMode=!objectName.isEmpty();

    QString placeholderText=tr("default");
    QRegExp digitsRegExp("\\d*", Qt::CaseSensitive, QRegExp::RegExp2);
    QRegExpValidator *intValidator=new QRegExpValidator(digitsRegExp, this);

    QFormLayout *form=new QFormLayout();

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
    form->addRow(tr("Start with"), startWithEditor);

    setLayout(form);

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

void SequenceCreatorPane::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    DbObjectCreatorPane::setQueryScheduler(queryScheduler);

    if(editMode){
        schemaList->addItem(IconUtil::getIcon("user"), schemaList->currentText());
        schemaList->setCurrentIndex(0);
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
    return QList<QueryListItem>();
}

QString SequenceCreatorPane::getSchemaName() const
{
    return schemaList->lineEdit()->text().trimmed().toUpper();
}

QString SequenceCreatorPane::getObjectName() const
{
    return sequenceNameEditor->text().trimmed().toUpper();
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
        info.cacheSize="";
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
}

void SequenceCreatorPane::disableControlsForEditMode()
{
    schemaList->setEnabled(false);
}
