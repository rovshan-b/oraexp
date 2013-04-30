#include "schemacompareroptionstab.h"
#include "util/widgethelper.h"
#include "connectivity/dbconnection.h"
#include "widgets/tableddlexportoptionswidget.h"
#include "widgets/storageddlexportoptionswidget.h"
#include "widgets/sourceddlexportoptionswidget.h"
#include "widgets/sequenceddlexportoptionswidget.h"
#include "widgets/sequenceddldiffoptionswidget.h"
#include <QtGui>

SchemaComparerOptionsTab::SchemaComparerOptionsTab(QWidget *parent) :
    DbObjectComparerOptionsTab(parent)
{
    QVBoxLayout *mainLayout=new QVBoxLayout();

    createOptionsPane(mainLayout);

    setLayout(mainLayout);
}

void SchemaComparerOptionsTab::setCanWrap(bool canWrap)
{
    sourceOptionsWidget->setCanWrap(canWrap,
                                    canWrap ?
                                        ""
                                      :
                                        tr("Either source database version is higher than target or one of database versions is lower than 10g"));
}

void SchemaComparerOptionsTab::setCanFlashbackArchive(bool canFlashbackArchive)
{
    QString notSupportedTooltip=tr("Not supported by source or target or both databases");
    if(canFlashbackArchive){
        etFlashbackArchive->setEnabled(true);
    }else{
        etFlashbackArchive->setToolTip(notSupportedTooltip);
    }
    ntOptionsWidget->setFlashbackEnabled(canFlashbackArchive, canFlashbackArchive ? "" : notSupportedTooltip);
}

void SchemaComparerOptionsTab::targetConnectionEstablished(DbConnection *sourceDb, DbConnection *targetDb)
{
    bool canWrap=sourceDb->supportsDbmsWrap() && targetDb->supportsDbmsWrap() && sourceDb->compareVersion(targetDb)<=0;
    setCanWrap(canWrap);
    setCanFlashbackArchive(sourceDb->supportsFlashbackArchive() && targetDb->supportsFlashbackArchive());
}

DbObjectComparisonOptions *SchemaComparerOptionsTab::getOptions()
{
    SchemaComparisonOptions *opt=new SchemaComparisonOptions();

    opt->tableDiffOptions.comments=etComments->isChecked();
    opt->tableDiffOptions.properties=etProperties->isChecked();
    opt->tableDiffOptions.flashbackArchive=etFlashbackArchive->isChecked();
    opt->tableDiffOptions.storage=etStorage->isChecked();
    opt->tableDiffOptions.iotProperties=etIOTProperties->isChecked();
    opt->tableDiffOptions.lobProperties=etLOBProperties->isChecked();
    opt->tableDiffOptions.columns=etColumns->isChecked();
    opt->tableDiffOptions.constraints=etConstraints->isChecked();
    opt->tableDiffOptions.indexes=etIndexes->isChecked();
    opt->tableDiffOptions.partitions=etPartitions->isChecked();
    opt->tableDiffOptions.externalProperties=etExternalProperties->isChecked();
    opt->tableDiffOptions.triggers=etTriggers->isChecked();
    opt->tableDiffOptions.grants=etGrants->isChecked();

    opt->tableCreateOptions = ntOptionsWidget->getOptions();

    NewDbObjectStorageOptions newObjectOptions = noOptionsWidget->getOptions();

    opt->tableCreateOptions.newObjectStorageOptions=newObjectOptions;
    opt->tableDiffOptions.newObjectOptions=newObjectOptions;

    opt->sourceCodeOptions = sourceOptionsWidget->getOptions();

    opt->sequenceCreateOptions = seqExportOptionWidget->getOptions();
    opt->sequenceDiffOptions = seqDiffOptionsWidget->getOptions();

    return opt;
}

void SchemaComparerOptionsTab::etOptionsChanged(int)
{
    etLOBProperties->setEnabled(etColumns->isChecked());
}

void SchemaComparerOptionsTab::createOptionsPane(QBoxLayout *layout)
{
    QVBoxLayout *optionsPaneLayout=new QVBoxLayout();

    QHBoxLayout *firstRowLayout=new QHBoxLayout();

    QVBoxLayout *firstColLayout=new QVBoxLayout();
    firstColLayout->setContentsMargins(0,0,0,0);
    firstRowLayout->addLayout(firstColLayout);
    firstRowLayout->setAlignment(firstColLayout, Qt::AlignTop|Qt::AlignLeft);

    createNewTableOptionsPane(firstColLayout);
    createNewObjectOptionsPane(firstColLayout);
    createSourceOptionsPane(firstColLayout);

    QVBoxLayout *secondColLayout=new QVBoxLayout();
    secondColLayout->setContentsMargins(0,0,0,0);
    firstRowLayout->addLayout(secondColLayout);

    createExistingTableOptionsPane(secondColLayout);
    createSequenceOptionsPane(secondColLayout);

    int minWidth=qMax(newTableOptionsGroupBox->sizeHint().width(), newObjectOptionsGroupBox->sizeHint().width());
    newTableOptionsGroupBox->setMinimumWidth(minWidth);
    newObjectOptionsGroupBox->setMinimumWidth(minWidth);
    sourceOptionsGroupBox->setMinimumWidth(minWidth);

    minWidth=qMax(existingTableOptionsGroupBox->sizeHint().width(), sourceOptionsGroupBox->sizeHint().width());
    minWidth=qMax(minWidth, sequenceOptionsGroupBox->sizeHint().width());

    existingTableOptionsGroupBox->setMinimumWidth(minWidth);
    sequenceOptionsGroupBox->setMinimumWidth(minWidth);

    optionsPaneLayout->addLayout(firstRowLayout);
    optionsPaneLayout->setAlignment(firstRowLayout, Qt::AlignLeft|Qt::AlignTop);

    optionsPaneLayout->addStretch();

    layout->addWidget(WidgetHelper::createScrollArea(optionsPaneLayout));
    layout->setAlignment(optionsPaneLayout, Qt::AlignTop|Qt::AlignLeft);
}

void SchemaComparerOptionsTab::createExistingTableOptionsPane(QBoxLayout *layout)
{
    existingTableOptionsGroupBox=new QGroupBox(tr("Existing tables"));

    QGridLayout *tableOptionsLayout=new QGridLayout();

    etComments=WidgetHelper::createCheckBox(tableOptionsLayout, 0, 0, tr("Comments"), true);
    etProperties=WidgetHelper::createCheckBox(tableOptionsLayout, 1, 0, tr("Properties"), false);
    etFlashbackArchive=WidgetHelper::createCheckBox(tableOptionsLayout, 2, 0, tr("Flashback archive"), false);
    etFlashbackArchive->setEnabled(false);
    etFlashbackArchive->setToolTip(tr("Will be enabled if both databases are version 11g or higher"));
    etStorage=WidgetHelper::createCheckBox(tableOptionsLayout, 3, 0, tr("Storage"), false);
    etIOTProperties=WidgetHelper::createCheckBox(tableOptionsLayout, 4, 0, tr("IOT Properties"), false);
    etColumns=WidgetHelper::createCheckBox(tableOptionsLayout, 5, 0, tr("Columns"), true);
    etLOBProperties=WidgetHelper::createCheckBox(tableOptionsLayout, 6, 0, tr("LOB Properties"), false);
    etConstraints=WidgetHelper::createCheckBox(tableOptionsLayout, 0, 1, tr("Constraints"), true);
    etIndexes=WidgetHelper::createCheckBox(tableOptionsLayout, 1, 1, tr("Indexes"), true);
    etPartitions=WidgetHelper::createCheckBox(tableOptionsLayout, 2, 1, tr("Partitions"), true);
    etExternalProperties=WidgetHelper::createCheckBox(tableOptionsLayout, 3, 1, tr("External properties"), true);
    etTriggers=WidgetHelper::createCheckBox(tableOptionsLayout, 4, 1, tr("Triggers"), true);
    etGrants=WidgetHelper::createCheckBox(tableOptionsLayout, 5, 1, tr("Grants"), false);

    existingTableOptionsGroupBox->setLayout(tableOptionsLayout);
    layout->addWidget(existingTableOptionsGroupBox);
    layout->setAlignment(existingTableOptionsGroupBox, Qt::AlignTop|Qt::AlignLeft);

    connect(etColumns, SIGNAL(stateChanged(int)), this, SLOT(etOptionsChanged(int)));
}

void SchemaComparerOptionsTab::createNewTableOptionsPane(QBoxLayout *layout)
{
    QVBoxLayout *optionsLayout = new QVBoxLayout();
    ntOptionsWidget = new TableDdlExportOptionsWidget();
    optionsLayout->addWidget(ntOptionsWidget);
    newTableOptionsGroupBox = WidgetHelper::createGroupBox(optionsLayout, tr("New tables"));

    layout->addWidget(newTableOptionsGroupBox);
    layout->setAlignment(newTableOptionsGroupBox, Qt::AlignTop|Qt::AlignLeft);    
}

void SchemaComparerOptionsTab::createNewObjectOptionsPane(QBoxLayout *layout)
{
    QVBoxLayout *optionsLayout = new QVBoxLayout();
    noOptionsWidget = new StorageDdlExportOptionsWidget();
    optionsLayout->addWidget(noOptionsWidget);
    newObjectOptionsGroupBox = WidgetHelper::createGroupBox(optionsLayout, tr("Storage for new objects"));

    layout->addWidget(newObjectOptionsGroupBox);
    layout->setAlignment(newObjectOptionsGroupBox, Qt::AlignTop|Qt::AlignLeft);
}

void SchemaComparerOptionsTab::createSourceOptionsPane(QBoxLayout *layout)
{
    QVBoxLayout *optionsLayout = new QVBoxLayout();
    sourceOptionsWidget = new SourceDdlExportOptionsWidget(true);
    optionsLayout->addWidget(sourceOptionsWidget);
    sourceOptionsGroupBox = WidgetHelper::createGroupBox(optionsLayout, tr("Source code"));

    layout->addWidget(sourceOptionsGroupBox);
    layout->setAlignment(sourceOptionsGroupBox, Qt::AlignTop|Qt::AlignLeft);
}

void SchemaComparerOptionsTab::createSequenceOptionsPane(QBoxLayout *layout)
{
    QVBoxLayout *optionsLayout=new QVBoxLayout();

    seqExportOptionWidget = new SequenceDdlExportOptionsWidget();
    seqDiffOptionsWidget = new SequenceDdlDiffOptionsWidget();

    optionsLayout->addWidget(seqExportOptionWidget);
    optionsLayout->addWidget(seqDiffOptionsWidget);

    sequenceOptionsGroupBox = WidgetHelper::createGroupBox(optionsLayout, tr("Sequences"));

    layout->addWidget(sequenceOptionsGroupBox);
    layout->setAlignment(sequenceOptionsGroupBox, Qt::AlignTop|Qt::AlignLeft);
}
/*
void SchemaComparerOptionsTab::createIndexOptionsPane(QBoxLayout *layout)
{
    indexOptionsGroupBox=new QGroupBox(tr("Indexes"));

    QVBoxLayout *indexOptionsLayout=new QVBoxLayout();

    indexPartitions=WidgetHelper::createCheckBox(indexOptionsLayout, tr("Global partitions"), true);
    indexLobStorage=WidgetHelper::createCheckBox(indexOptionsLayout, tr("LOB storage"), true);
    indexTablespace=WidgetHelper::createCheckBox(indexOptionsLayout, tr("Tablespace"), false);

    indexOptionsGroupBox->setLayout(indexOptionsLayout);
    layout->addWidget(indexOptionsGroupBox);
}
*/
