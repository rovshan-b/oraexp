#ifndef SCHEMAEXPORTEROPTIONSTAB_H
#define SCHEMAEXPORTEROPTIONSTAB_H

#include "connection_page/db_object_comparer/tabs/dbobjectcompareroptionstab.h"

class QGroupBox;
class TableDdlExportOptionsWidget;
class StorageDdlExportOptionsWidget;
class SourceDdlExportOptionsWidget;
class SequenceDdlExportOptionsWidget;
class FileSelectorWidget;
class FileEncodingWidget;
class QFormLayout;

class SchemaExporterOptionsTab : public DbObjectComparerOptionsTab
{
    Q_OBJECT
public:
    explicit SchemaExporterOptionsTab(QWidget *parent = 0);

    virtual void setQueryScheduler(IQueryScheduler *queryScheduler);

    virtual DbObjectComparisonOptions *getOptions();

    void createFileWidgets(QFormLayout *form);

    virtual bool validate();
    
private:
    void createOptionsPane(QVBoxLayout *layout);

    void createTableOptionsPane(QVBoxLayout *layout);
    void createStorageOptionsPane(QVBoxLayout *layout);
    void createSourceOptionsPane(QVBoxLayout *layout);
    void createSequenceOptionsPane(QVBoxLayout *layout);

    QGroupBox *tableOptionsGroupBox;
    TableDdlExportOptionsWidget *tableOptionsWidget;

    QGroupBox *storageOptionsGroupBox;
    StorageDdlExportOptionsWidget *storageOptionsWidget;

    QGroupBox *sourceOptionsGroupBox;
    SourceDdlExportOptionsWidget *sourceOptionsWidget;

    QGroupBox *sequenceOptionsGroupBox;
    SequenceDdlExportOptionsWidget *sequenceOptionsWidget;

    FileSelectorWidget *filenameEditor;
    FileEncodingWidget *encodingWidget;
    
};

#endif // SCHEMAEXPORTEROPTIONSTAB_H
