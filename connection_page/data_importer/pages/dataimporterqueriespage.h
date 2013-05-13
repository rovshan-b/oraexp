#ifndef DATAIMPORTERQUERIESPAGE_H
#define DATAIMPORTERQUERIESPAGE_H

#include <QWizardPage>

class CodeEditor;
class QGroupBox;
class QVBoxLayout;

class DataImporterQueriesPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit DataImporterQueriesPage(QWidget *parent = 0);
    
    virtual void initializePage();

private:
    QString currentSchemaName;
    QString currentTableName;
    QString currentFileName;

    QGroupBox *beforeImportGroupBox;
    CodeEditor *beforeImportQueryEditor;

    CodeEditor *importQueryEditor;

    QGroupBox *afterImportGroupBox;
    CodeEditor *afterImportQueryEditor;

    void createBeforeImportQueryBox(QVBoxLayout *layout);
    void createImportQueryBox(QVBoxLayout *layout);
    void createAfterImportQueryBox(QVBoxLayout *layout);
    
};

#endif // DATAIMPORTERQUERIESPAGE_H
