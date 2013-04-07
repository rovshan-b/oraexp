#include "schemacompareroptionstab.h"
#include "util/widgethelper.h"
#include "connectivity/dbconnection.h"
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
    sourceWrap->setEnabled(canWrap);
    if(canWrap){
        sourceWrap->setToolTip("");
    }else{
        sourceWrap->setToolTip(tr("Either source database version is higher than target or one of database versions is lower than 10g"));
    }
}

void SchemaComparerOptionsTab::setCanFlashbackArchive(bool canFlashbackArchive)
{
    if(canFlashbackArchive){
        ntFlashbackArchive->setEnabled(true);
        etFlashbackArchive->setEnabled(true);
    }else{
        QString tooltip=tr("Not supported by source or target or both databases");
        ntFlashbackArchive->setToolTip(tooltip);
        etFlashbackArchive->setToolTip(tooltip);
    }
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

    opt->tableCreateOptions.properties=ntProperties->isChecked();
    opt->tableCreateOptions.flashbackArchive=ntFlashbackArchive->isChecked();
    opt->tableCreateOptions.iotProperties=ntIOTProperties->isChecked();
    opt->tableCreateOptions.lobProperties=ntLobProperties->isChecked();
    opt->tableCreateOptions.indexes=ntIndexes->isChecked();
    opt->tableCreateOptions.triggers=ntTriggers->isChecked();
    opt->tableCreateOptions.grants=ntGrants->isChecked();

    NewDbObjectDdlOptions newObjectOptions;
    newObjectOptions.storage=noStorage->isChecked();
    newObjectOptions.tablespaceOnly=noTablespaceOnly->isChecked();
    newObjectOptions.tablespaceName=noTablespaceName->text().trimmed().toUpper();
    newObjectOptions.iotOverflowTablespaceName=noIOTOverflowTablespaceName->text().trimmed().toUpper();
    newObjectOptions.indexTablespaceName=noIndexTablespaceName->text().trimmed().toUpper();
    newObjectOptions.lobTablespaceName=noLobTablespaceName->text().trimmed().toUpper();
    newObjectOptions.partitionsTablespaceName=noPartitionsTablespaceName->text().trimmed().toUpper();

    opt->tableCreateOptions.newObjectOptions=newObjectOptions;
    opt->tableDiffOptions.newObjectOptions=newObjectOptions;

    opt->sourceCodeOptions.ignoreWhitespace=sourceIgnoreWhitespace->isChecked();
    opt->sourceCodeOptions.wrap=sourceWrap->isChecked();

    opt->sequenceCreateOptions.setInitialValue=seqSetStartVal->isChecked();
    opt->sequenceDiffOptions.updateCurrVal=seqUpdateCurrval->isChecked();

    return opt;
}

void SchemaComparerOptionsTab::noOptionsChanged(int)
{
    bool storageChecked=noStorage->isChecked();
    noTablespaceOnly->setEnabled(storageChecked);
    noTablespaceName->setEnabled(storageChecked);
    noIOTOverflowTablespaceName->setEnabled(storageChecked);
    noIndexTablespaceName->setEnabled(storageChecked);
    noLobTablespaceName->setEnabled(storageChecked);
    noPartitionsTablespaceName->setEnabled(storageChecked);
}

void SchemaComparerOptionsTab::etOptionsChanged(int)
{
    etLOBProperties->setEnabled(etColumns->isChecked());
}

void SchemaComparerOptionsTab::sourceWrapCheckBoxStateChanged(int state)
{
    sourceIgnoreWhitespace->setEnabled(state!=Qt::Checked);
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

    QWidget *scrollWidget=new QWidget();
    scrollWidget->setContentsMargins(0,0,0,0);
    scrollWidget->setLayout(optionsPaneLayout);

    QScrollArea *scrollArea=new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setBackgroundRole(QPalette::Base);
    scrollArea->setWidget(scrollWidget);

    layout->addWidget(scrollArea);
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
    newTableOptionsGroupBox=new QGroupBox(tr("New tables"));

    QGridLayout *tableOptionsLayout=new QGridLayout();

    ntProperties=WidgetHelper::createCheckBox(tableOptionsLayout, 0, 0, tr("Properties"), false);
    ntFlashbackArchive=WidgetHelper::createCheckBox(tableOptionsLayout, 1, 0, tr("Flashback archive"), false);
    ntFlashbackArchive->setEnabled(false);
    ntFlashbackArchive->setToolTip(tr("Will be enabled if both databases are version 11g or higher"));
    ntIOTProperties=WidgetHelper::createCheckBox(tableOptionsLayout, 2, 0, tr("IOT properties"), true);
    ntLobProperties=WidgetHelper::createCheckBox(tableOptionsLayout, 3, 0, tr("LOB properties"), false);
    ntIndexes=WidgetHelper::createCheckBox(tableOptionsLayout, 0, 1, tr("Indexes"), true);

    ntTriggers=WidgetHelper::createCheckBox(tableOptionsLayout, 1, 1, tr("Triggers"), true);
    ntGrants=WidgetHelper::createCheckBox(tableOptionsLayout, 2, 1, tr("Grants"), false);

    newTableOptionsGroupBox->setLayout(tableOptionsLayout);
    layout->addWidget(newTableOptionsGroupBox);
    layout->setAlignment(newTableOptionsGroupBox, Qt::AlignTop|Qt::AlignLeft);    
}

void SchemaComparerOptionsTab::createNewObjectOptionsPane(QBoxLayout *layout)
{
    newObjectOptionsGroupBox=new QGroupBox(tr("Storage for new objects"));

    QGridLayout *newObjectOptionsLayout=new QGridLayout();

    noStorage=WidgetHelper::createCheckBox(newObjectOptionsLayout, 0, 0, tr("Generate storage DDL"), false);
    noTablespaceOnly=WidgetHelper::createCheckBox(newObjectOptionsLayout, 0, 1, tr("Tablespace only"), true);

    noTablespaceName=new QLineEdit();
    newObjectOptionsLayout->addWidget(new QLabel(tr("Tablespace for tables")), 2, 0);
    newObjectOptionsLayout->addWidget(noTablespaceName, 2, 1);

    noIOTOverflowTablespaceName=new QLineEdit();
    newObjectOptionsLayout->addWidget(new QLabel(tr("IOT overflow tablespace")),3, 0);
    newObjectOptionsLayout->addWidget(noIOTOverflowTablespaceName, 3, 1);

    noIndexTablespaceName=new QLineEdit();
    newObjectOptionsLayout->addWidget(new QLabel(tr("Index tablespace")), 4, 0);
    newObjectOptionsLayout->addWidget(noIndexTablespaceName, 4, 1);

    noLobTablespaceName=new QLineEdit();
    newObjectOptionsLayout->addWidget(new QLabel(tr("LOB tablespace")), 5, 0);
    newObjectOptionsLayout->addWidget(noLobTablespaceName, 5, 1);

    noPartitionsTablespaceName=new QLineEdit();
    newObjectOptionsLayout->addWidget(new QLabel(tr("Partition tablespace")), 6, 0);
    newObjectOptionsLayout->addWidget(noPartitionsTablespaceName, 6, 1);


    newObjectOptionsGroupBox->setLayout(newObjectOptionsLayout);
    layout->addWidget(newObjectOptionsGroupBox);
    layout->setAlignment(newObjectOptionsGroupBox, Qt::AlignTop|Qt::AlignLeft);

    connect(noStorage, SIGNAL(stateChanged(int)), this, SLOT(noOptionsChanged(int)));
    noOptionsChanged(noStorage->checkState());
}

void SchemaComparerOptionsTab::createSourceOptionsPane(QBoxLayout *layout)
{
    sourceOptionsGroupBox=new QGroupBox(tr("Source code"));

    QGridLayout *sourceOptionsLayout=new QGridLayout();

    sourceIgnoreWhitespace=WidgetHelper::createCheckBox(sourceOptionsLayout, 0, 0, tr("Ignore whitespace"), true);
    sourceWrap=WidgetHelper::createCheckBox(sourceOptionsLayout, 1, 0, tr("Wrap"), false);
    sourceWrap->setEnabled(false);
    sourceWrap->setToolTip(tr("Will be enabled upon connecting to target database if version check is successfull"));

    sourceOptionsGroupBox->setLayout(sourceOptionsLayout);
    layout->addWidget(sourceOptionsGroupBox);
    layout->setAlignment(sourceOptionsGroupBox, Qt::AlignTop|Qt::AlignLeft);

    connect(sourceWrap, SIGNAL(stateChanged(int)), this, SLOT(sourceWrapCheckBoxStateChanged(int)));
}

void SchemaComparerOptionsTab::createSequenceOptionsPane(QBoxLayout *layout)
{
    sequenceOptionsGroupBox=new QGroupBox(tr("Sequences"));

    QGridLayout *sequenceOptionsLayout=new QGridLayout();

    seqSetStartVal=WidgetHelper::createCheckBox(sequenceOptionsLayout, 0, 0, tr("Set start value"), false);
    seqUpdateCurrval=WidgetHelper::createCheckBox(sequenceOptionsLayout, 1, 0, tr("Update CURRVAL"), false);

    sequenceOptionsGroupBox->setLayout(sequenceOptionsLayout);
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
