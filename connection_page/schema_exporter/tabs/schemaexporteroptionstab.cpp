#include "schemaexporteroptionstab.h"
#include "widgets/tableddlexportoptionswidget.h"
#include "widgets/storageddlexportoptionswidget.h"
#include "widgets/sourceddlexportoptionswidget.h"
#include "widgets/sequenceddlexportoptionswidget.h"
#include "widgets/fileselectorwidget.h"
#include "widgets/fileencodingwidget.h"
#include "beans/schemaexportoptions.h"
#include "util/widgethelper.h"
#include "connectivity/dbconnection.h"
#include "defines.h"
#include <QtGui>

SchemaExporterOptionsTab::SchemaExporterOptionsTab(QWidget *parent) :
    DbObjectComparerOptionsTab(parent), filenameEditor(0)
{
    QVBoxLayout *mainLayout=new QVBoxLayout();

    createOptionsPane(mainLayout);

    setLayout(mainLayout);
}

void SchemaExporterOptionsTab::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    bool wrapEnabled = queryScheduler->getDb()->supportsDbmsWrap();
    bool flashbackEnabled = queryScheduler->getDb()->supportsFlashbackArchive();

    sourceOptionsWidget->setCanWrap(wrapEnabled, wrapEnabled ? "" : tr(NOT_SUPPORTED_BY_DB_MSG));
    tableOptionsWidget->setFlashbackEnabled(flashbackEnabled, flashbackEnabled ? "" : tr(NOT_SUPPORTED_BY_DB_MSG));
}

DbObjectComparisonOptions *SchemaExporterOptionsTab::getOptions()
{
    SchemaExportOptions *opt = new SchemaExportOptions();

    opt->filename = filenameEditor->fileName();
    opt->encoding = encodingWidget->encoding();
    opt->bom = encodingWidget->bom();

    opt->tableOptions = tableOptionsWidget->getOptions();
    opt->tableOptions.newObjectStorageOptions = storageOptionsWidget->getOptions();
    opt->sourceCodeOptions = sourceOptionsWidget->getOptions();
    opt->sequenceOptions = sequenceOptionsWidget->getOptions();

    return opt;
}

void SchemaExporterOptionsTab::createOptionsPane(QVBoxLayout *layout)
{
    QHBoxLayout *rowsLayout = new QHBoxLayout();
    QVBoxLayout *firstColLayout = new QVBoxLayout();
    QVBoxLayout *secondColLayout = new QVBoxLayout();

    createTableOptionsPane(firstColLayout);
    createStorageOptionsPane(firstColLayout);
    createSourceOptionsPane(secondColLayout);
    createSequenceOptionsPane(secondColLayout);

    firstColLayout->addStretch();
    rowsLayout->addLayout(firstColLayout);
    secondColLayout->addStretch();
    rowsLayout->addLayout(secondColLayout);
    rowsLayout->addStretch();

    rowsLayout->setAlignment(firstColLayout, Qt::AlignTop|Qt::AlignLeft);
    rowsLayout->setAlignment(secondColLayout, Qt::AlignTop|Qt::AlignLeft);

    int minWidth = qMax(tableOptionsGroupBox->sizeHint().width(), storageOptionsGroupBox->sizeHint().width());
    tableOptionsGroupBox->setMinimumWidth(minWidth);
    storageOptionsGroupBox->setMinimumWidth(minWidth);

    minWidth = qMax(sourceOptionsWidget->sizeHint().width(), sequenceOptionsWidget->sizeHint().width());
    sourceOptionsWidget->setMinimumWidth(minWidth);
    sequenceOptionsWidget->setMinimumWidth(minWidth);

    layout->addWidget(WidgetHelper::createScrollArea(rowsLayout));
}

void SchemaExporterOptionsTab::createTableOptionsPane(QVBoxLayout *layout)
{
    tableOptionsWidget = new TableDdlExportOptionsWidget();
    tableOptionsGroupBox = WidgetHelper::createGroupBox(tableOptionsWidget, tr("Tables"));

    layout->addWidget(tableOptionsGroupBox);
    layout->setAlignment(tableOptionsGroupBox, Qt::AlignTop|Qt::AlignLeft);
}

void SchemaExporterOptionsTab::createStorageOptionsPane(QVBoxLayout *layout)
{
    storageOptionsWidget = new StorageDdlExportOptionsWidget();
    storageOptionsGroupBox = WidgetHelper::createGroupBox(storageOptionsWidget, tr("Storage"));

    layout->addWidget(storageOptionsGroupBox);
    layout->setAlignment(storageOptionsGroupBox, Qt::AlignTop|Qt::AlignLeft);
}

void SchemaExporterOptionsTab::createSourceOptionsPane(QVBoxLayout *layout)
{
    sourceOptionsWidget = new SourceDdlExportOptionsWidget(false);
    sourceOptionsGroupBox = WidgetHelper::createGroupBox(sourceOptionsWidget, tr("Source code"));

    layout->addWidget(sourceOptionsGroupBox);
    layout->setAlignment(sourceOptionsGroupBox, Qt::AlignTop|Qt::AlignLeft);
}

void SchemaExporterOptionsTab::createSequenceOptionsPane(QVBoxLayout *layout)
{
    sequenceOptionsWidget = new SequenceDdlExportOptionsWidget();
    sequenceOptionsGroupBox = WidgetHelper::createGroupBox(sequenceOptionsWidget, tr("Sequences"));

    layout->addWidget(sequenceOptionsGroupBox);
    layout->setAlignment(sequenceOptionsGroupBox, Qt::AlignTop|Qt::AlignLeft);
}

void SchemaExporterOptionsTab::createFileWidgets(QFormLayout *form)
{
    Q_ASSERT(filenameEditor==0);

    filenameEditor = new FileSelectorWidget(FileSelectorWidget::Save);
    filenameEditor->setDefaultSuffix("sql");
    form->addRow(tr("File name"), filenameEditor);

    encodingWidget = new FileEncodingWidget();
    form->addRow(tr("Encoding"), encodingWidget);
}

bool SchemaExporterOptionsTab::validate()
{
    if(filenameEditor->fileName().isEmpty()){
        QMessageBox::critical(this, tr("Validation error"), tr("Please, enter the filename."));
        return false;
    }

    return true;
}
