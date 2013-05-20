#ifndef DATAIMPORTERQUERIESPAGE_H
#define DATAIMPORTERQUERIESPAGE_H

#include "connection_page/connectionpagewizardpage.h"

class CodeEditor;
class QGroupBox;
class QVBoxLayout;
class ColumnMapping;

class DataImporterQueriesPage : public ConnectionPageWizardPage
{
    Q_OBJECT
public:
    explicit DataImporterQueriesPage(QWidget *parent = 0);
    
    virtual void initializePage();
    virtual void cleanupPage();
    virtual bool validatePage();

private:
    QString currentSchemaName;
    QString currentTableName;

    QGroupBox *beforeImportGroupBox;
    CodeEditor *beforeImportQueryEditor;

    CodeEditor *importQueryEditor;

    QGroupBox *afterImportGroupBox;
    CodeEditor *afterImportQueryEditor;

    void createBeforeImportQueryBox(QVBoxLayout *layout);
    void createImportQueryBox(QVBoxLayout *layout);
    void createAfterImportQueryBox(QVBoxLayout *layout);

    static QString generateInsertStatement(const QString &schemaName, const QString &tableName,
                                           const QList<ColumnMapping *> &columnMappings);

    
};

#endif // DATAIMPORTERQUERIESPAGE_H
