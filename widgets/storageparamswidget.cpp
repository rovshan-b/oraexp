#include "storageparamswidget.h"
#include "extentsizeeditorwidget.h"
#include "widgets/tablespacelistcombobox.h"
#include "util/strutil.h"
#include "util/widgethelper.h"
#include "connectivity/dbconnection.h"
#include "interfaces/iqueryscheduler.h"
#include <QtGui>

StorageParamsWidget::StorageParamsWidget(IQueryScheduler *queryScheduler,
                                         bool isEditMode,
                                         bool configureForIndex,
                                         bool configureForLob,
                                         QWidget *parent) :
    QWidget(parent),
    tablespaceCombo(0),
    pctFreeEditor(0),
    pctUsedEditor(0),
    initTransEditor(0),
    maxTransEditor(0),
    maxSizeWidget(0)
{
    //const int editorWidth=380;

    QIntValidator *intValidator=new QIntValidator(this);
    intValidator->setBottom(0);

    if(!configureForLob){
        tablespaceCombo=new TablespaceListComboBox(queryScheduler);
        tablespaceCombo->comboBox()->setCurrentIndex(0);
        tablespaceCombo->setEnabled(!isEditMode);

        pctFreeEditor=new QLineEdit();
        pctFreeEditor->setValidator(intValidator);

        pctUsedEditor=new QLineEdit();
        pctUsedEditor->setValidator(intValidator);
        //if(!configureForIndex){
        //    pctUsedEditor->setEnabled(false);
        //}

        if(configureForIndex && isEditMode){
            pctFreeEditor->setEnabled(false);
            pctUsedEditor->setEnabled(false);
        }

        initTransEditor=new QLineEdit();
        initTransEditor->setValidator(intValidator);
        maxTransEditor=new QLineEdit();
        maxTransEditor->setValidator(intValidator);
    }

    initExtent=new ExtentSizeEditorWidget(true, false);
    initExtent->setEnabled(!isEditMode);

    nextExtent=new ExtentSizeEditorWidget(true, false);
    nextExtent->setEnabled(!isEditMode);

    pctIncreaseEditor=new QLineEdit();
    pctIncreaseEditor->setValidator(intValidator);
    pctIncreaseEditor->setEnabled(!isEditMode);

    minExtentsEditor=new QLineEdit();
    minExtentsEditor->setValidator(intValidator);
    minExtentsEditor->setEnabled(!isEditMode);

    maxExtentsWidget=new ExtentSizeEditorWidget(false, true);
    maxExtentsWidget->setEnabled(!isEditMode);

    maxSizeWidget=new ExtentSizeEditorWidget(true, true);
    if(!queryScheduler->getDb()->supportsStorageMaxsizeClause()){
        maxSizeWidget->setEnabled(false);
        maxSizeWidget->setToolTip(tr("Oracle 11g or later"));
    }

    freeListsEditor=new QLineEdit();
    freeListsEditor->setValidator(intValidator);

    freeListGroupsEditor=new QLineEdit();
    freeListGroupsEditor->setValidator(intValidator);
    freeListGroupsEditor->setEnabled(!isEditMode);

    bufferPoolCombo=new QComboBox();
    bufferPoolCombo->addItem("");
    bufferPoolCombo->addItem("KEEP");
    bufferPoolCombo->addItem("RECYCLE");


    QVBoxLayout *mainLayout=new QVBoxLayout();

    if(!configureForLob){
        QGridLayout *physicalAttributesLayout=new QGridLayout();

        physicalAttributesLayout->addWidget(new QLabel(tr("Tablespace")), 0, 0);
        physicalAttributesLayout->addWidget(tablespaceCombo, 0, 1, 1, 3);

        physicalAttributesLayout->addWidget(new QLabel(tr("% Free")), 1, 0);
        physicalAttributesLayout->addWidget(pctFreeEditor, 1, 1);

        physicalAttributesLayout->addWidget(new QLabel(tr("% Used")), 2, 0);
        physicalAttributesLayout->addWidget(pctUsedEditor, 2, 1);

        physicalAttributesLayout->addWidget(new QLabel(tr("Init Trans")), 1, 2);
        physicalAttributesLayout->addWidget(initTransEditor, 1, 3);

        physicalAttributesLayout->addWidget(new QLabel(tr("Max Trans")), 2, 2);
        physicalAttributesLayout->addWidget(maxTransEditor, 2, 3);

        mainLayout->addLayout(physicalAttributesLayout);
    }

    QFormLayout *form=new QFormLayout();
    form->addRow(tr("Initial Extent"), initExtent);
    form->addRow(tr("Next Extent"), nextExtent);
    form->addRow(tr("% Increase"), pctIncreaseEditor);
    form->addRow(tr("Min Extents"), minExtentsEditor);
    form->addRow(tr("Max Extents"), maxExtentsWidget);

    form->addRow(tr("Max Size"), maxSizeWidget);

    form->addRow(tr("Free Lists"), freeListsEditor);
    form->addRow(tr("Free List Groups"), freeListGroupsEditor);
    form->addRow(tr("Buffer Pool"), bufferPoolCombo);

    if(!configureForLob){
        QGroupBox *groupBox=new QGroupBox(tr("Storage"));
        groupBox->setLayout(form);
        mainLayout->addWidget(groupBox);
    }else{
        mainLayout->addLayout(form);
    }

    setLayout(mainLayout);
}

StorageParams StorageParamsWidget::getStorageParams() const
{
    StorageParams params;

    if(tablespaceCombo!=0){
        params.tablespaceName=tablespaceCombo->lineEdit()->text().trimmed();
    }

    if(pctFreeEditor!=0){
        params.pctFree=getEditorIntValue(pctFreeEditor->text());
        params.pctUsed=getEditorIntValue(pctUsedEditor->text());
        params.initTrans=getEditorIntValue(initTransEditor->text());
        params.maxTrans=getEditorIntValue(maxTransEditor->text());
    }

    params.initialExtent=getEditorIntValue(initExtent->text());
    params.initialExtentUnit=initExtent->unit();

    params.nextExtent=getEditorIntValue(nextExtent->text());
    params.nextExtentUnit=nextExtent->unit();

    params.pctIncrease=getEditorIntValue(pctIncreaseEditor->text());
    params.minExtents=getEditorIntValue(minExtentsEditor->text());

    params.maxExtents=getEditorIntValue(maxExtentsWidget->text());
    params.unlimitedMaxExtents=maxExtentsWidget->isUnlimited();

    //if(maxSizeWidget!=0){
        params.maxSize=getEditorIntValue(maxSizeWidget->text());
        params.maxSizeUnit=maxSizeWidget->unit();
        params.unlimitedMaxSize=maxSizeWidget->isUnlimited();
    //}

    params.freeLists=getEditorIntValue(freeListsEditor->text());
    params.freeListGroups=getEditorIntValue(freeListGroupsEditor->text());

    params.bufferPool=bufferPoolCombo->currentText();

    return params;
}

void StorageParamsWidget::setStorageParams(const StorageParams &params)
{
   if(tablespaceCombo!=0){
       WidgetHelper::setComboBoxText(tablespaceCombo->comboBox(), params.tablespaceName);
   }

   if(pctFreeEditor!=0){
       pctFreeEditor->setText(getEditorStrValue(params.pctFree));
       pctUsedEditor->setText(getEditorStrValue(params.pctUsed));
       initTransEditor->setText(getEditorStrValue(params.initTrans));
       maxTransEditor->setText(getEditorStrValue(params.maxTrans));
   }

   initExtent->setText(getEditorStrValue(params.initialExtent));
   initExtent->setUnit(params.initialExtentUnit);

   nextExtent->setText(getEditorStrValue(params.nextExtent));
   nextExtent->setUnit(params.nextExtentUnit);

   pctIncreaseEditor->setText(getEditorStrValue(params.pctIncrease));
   minExtentsEditor->setText(getEditorStrValue(params.minExtents));

   maxExtentsWidget->setText(getEditorStrValue(params.maxExtents));
   maxExtentsWidget->setUnlimited(params.unlimitedMaxExtents);

   //if(maxSizeWidget!=0){
       maxSizeWidget->setText(getEditorStrValue(params.maxSize));
       maxSizeWidget->setUnit(params.maxSizeUnit);
       maxSizeWidget->setUnlimited(params.unlimitedMaxSize);
   //}

   freeListsEditor->setText(getEditorStrValue(params.freeLists));
   freeListGroupsEditor->setText(getEditorStrValue(params.freeListGroups));

   bufferPoolCombo->setCurrentIndex(bufferPoolCombo->findText(params.bufferPool));
}
