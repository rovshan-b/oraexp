#ifndef DATAIMPORTERCSVOPTIONSPAGE_H
#define DATAIMPORTERCSVOPTIONSPAGE_H

#include <QWizardPage>
#include "../importers/csvimporter.h"
#include "interfaces/idataimportlistener.h"

class QVBoxLayout;
class QComboBox;
class QSpinBox;
class QLineEdit;
class DataTable;
class QStandardItemModel;

class DataImporterCsvOptionsPage : public QWizardPage, public IDataImportListener
{
    Q_OBJECT
public:
    explicit DataImporterCsvOptionsPage(QWidget *parent = 0);

    virtual void initializePage ();

    virtual void headerAvailable(const QStringList &headerTitles);
    virtual void rowAvailable(const QStringList &values);

    QStandardItemModel *getDataPreviewModel() const;

private slots:
    void setEncoding(bool refreshData = true);
    void setDelimiter(bool refreshData = true);
    void setEnclosure(bool refreshData = true);
    void setSkipRows(bool refreshData = true);
    void setHeaderOption(bool refreshData = true);
    
private:
    QComboBox *fileEncodingComboBox;
    QComboBox *delimiterComboBox;
    QSpinBox *skipRowsSpinBox;
    QComboBox *headerComboBox;
    QLineEdit *enclosureEditor;

    DataTable *previewTable;
    QStandardItemModel *tableModel;

    CsvImporter importer;

    void createForm(QVBoxLayout *mainLayout);

    void previewData();
    
};

#endif // DATAIMPORTERCSVOPTIONSPAGE_H
