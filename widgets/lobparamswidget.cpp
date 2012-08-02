#include "lobparamswidget.h"
#include "storageparamswidget.h"
#include "widgets/tablespacelistcombobox.h"
#include "util/strutil.h"
#include "util/widgethelper.h"
#include "connectivity/dbconnection.h"
#include "interfaces/iqueryscheduler.h"
#include "defines.h"
#include <QtGui>

#include <iostream>
using namespace std;

LobParamsWidget::LobParamsWidget(IQueryScheduler *queryScheduler, bool isEditMode, QWidget *parent) :
    QWidget(parent), queryScheduler(queryScheduler), isEditMode(isEditMode)
{
    QTabWidget *tab=new QTabWidget();

    QHBoxLayout *layout=new QHBoxLayout();

    QIntValidator *intValidator=new QIntValidator(this);
    intValidator->setBottom(0);

    QFormLayout *form=new QFormLayout();

    segmentEditor=new QLineEdit();
    segmentEditor->setMaxLength(MAX_IDENTIFIER_LENGTH);
    if(isEditMode){
        segmentEditor->setEnabled(false);
    }

    tablespaceCombo=new TablespaceListComboBox(queryScheduler);
    tablespaceCombo->comboBox()->insertItem(0, "");
    tablespaceCombo->comboBox()->setCurrentIndex(0);
    if(isEditMode){
        tablespaceCombo->setEnabled(false);
    }

    cacheCombo=new QComboBox();
    cacheCombo->addItem("");
    cacheCombo->addItem("CACHE");
    cacheCombo->addItem("NOCACHE");
    cacheCombo->addItem("CACHE READS");

    loggingCombo=new QComboBox();
    loggingCombo->addItem("");
    loggingCombo->addItem(tr("Enable"));
    loggingCombo->addItem(tr("Disable"));
    loggingCombo->setEnabled(false);

    chunkEditor=new QLineEdit();
    chunkEditor->setValidator(intValidator);
    if(isEditMode){
        chunkEditor->setEnabled(false);
    }
    pctVersionEditor=new QLineEdit();
    pctVersionEditor->setValidator(intValidator);

    freePoolsEditor=new QLineEdit();
    freePoolsEditor->setValidator(intValidator);

    storeInRowCheckBox=new QCheckBox();
    if(isEditMode){
        storeInRowCheckBox->setEnabled(false);
    }

    retentionCheckBox=new QCheckBox();

    form->addRow(tr("Segment"), segmentEditor);
    form->addRow(tr("Tablespace"), tablespaceCombo);
    form->addRow(tr("Cache"), cacheCombo);
    form->addRow(tr("Logging"), loggingCombo);
    form->addRow(tr("Chunk"), chunkEditor);
    form->addRow(tr("% Version"), pctVersionEditor);
    form->addRow(tr("Free Pools"), freePoolsEditor);
    form->addRow(tr("Store in row"), storeInRowCheckBox);
    form->addRow(tr("Retention"), retentionCheckBox);

    QWidget *lobTab=new QWidget();
    lobTab->setLayout(form);
    tab->addTab(lobTab, tr("LOB"));

    storageParamsWidget=new StorageParamsWidget(queryScheduler, isEditMode, false, true);
    tab->addTab(storageParamsWidget, tr("Storage"));


    layout->addWidget(tab);
    setLayout(layout);

    connect(cacheCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(cacheComboChanged(int)));
}

void LobParamsWidget::cacheComboChanged(int index)
{
    loggingCombo->setEnabled(index==2 || index==3);
    if(!loggingCombo->isEnabled()){
        loggingCombo->setCurrentIndex(0);
    }
}

LobParams LobParamsWidget::getLobParams() const
{
    LobParams params;

    params.segment=segmentEditor->text().trimmed();
    params.tablespaceName=tablespaceCombo->lineEdit()->text().trimmed();
    params.cache=cacheCombo->currentIndex();
    params.logging=loggingCombo->currentIndex();
    params.chunk=getEditorIntValue(chunkEditor->text().trimmed());
    params.pctVersion=getEditorIntValue(pctVersionEditor->text().trimmed());
    params.freePools=getEditorIntValue(freePoolsEditor->text().trimmed());
    params.enableStorageInRow=storeInRowCheckBox->isChecked();
    params.enableRetention=retentionCheckBox->isChecked();

    params.storageParams=storageParamsWidget->getStorageParams();

    return params;
}

void LobParamsWidget::setLobParams(const LobParams &params)
{
    segmentEditor->setText(params.segment);
    WidgetHelper::setComboBoxText(tablespaceCombo->comboBox(), params.tablespaceName);
    cacheCombo->setCurrentIndex(params.cache);
    loggingCombo->setCurrentIndex(params.logging);
    chunkEditor->setText(getEditorStrValue(params.chunk));
    pctVersionEditor->setText(getEditorStrValue(params.pctVersion));
    freePoolsEditor->setText(getEditorStrValue(params.freePools));
    storeInRowCheckBox->setChecked(params.enableStorageInRow);
    retentionCheckBox->setChecked(params.enableRetention);

    storageParamsWidget->setStorageParams(params.storageParams);

    if(isEditMode && retentionCheckBox->isChecked() && !queryScheduler->getDb()->supportsChangingLobRetention()){
        retentionCheckBox->setEnabled(false);
    }
}
