#ifndef DATAIMPORTERCSVOPTIONSPAGE_H
#define DATAIMPORTERCSVOPTIONSPAGE_H

#include <QWizardPage>
#include "../importers/csvimporter.h"

class QVBoxLayout;
class QComboBox;
class QSpinBox;
class QLineEdit;
class DataTable;

class DataImporterCsvOptionsPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit DataImporterCsvOptionsPage(QWidget *parent = 0);

    virtual void initializePage ();

private slots:
    void setEncoding();
    void setDelimiter();
    void setEnclosure();
    
private:
    QComboBox *fileEncodingComboBox;
    QComboBox *delimiterComboBox;
    QSpinBox *skipRowsSpinBox;
    QComboBox *headerComboBox;
    QLineEdit *enclosureEditor;

    DataTable *previewTable;

    CsvImporter importer;

    void createForm(QVBoxLayout *mainLayout);

    void previewData();
    
};

#endif // DATAIMPORTERCSVOPTIONSPAGE_H
