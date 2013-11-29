#ifndef DATAIMPORTERCSVOPTIONSPAGE_H
#define DATAIMPORTERCSVOPTIONSPAGE_H

#include "connection_page/connectionpagewizardpage.h"
#include "../importers/csvimporter.h"
#include "interfaces/idataimportlistener.h"

class QVBoxLayout;
class QComboBox;
class QSpinBox;
class QLineEdit;
class QCheckBox;
class DataTable;
class QStandardItemModel;

class DataImporterCsvOptionsPage : public ConnectionPageWizardPage, public IDataImportListener
{
    Q_OBJECT
public:
    explicit DataImporterCsvOptionsPage(DbUiManager *uiManager, QWidget *parent = 0);

    virtual void initializePage ();

    virtual void headerAvailable(const QStringList &headerTitles);
    virtual void rowAvailable(const QStringList &values);

    QStandardItemModel *getDataPreviewModel() const;
    CsvImporter *getImporter();

private slots:
    void setEncoding(bool refreshData = true);
    void setDelimiter(bool refreshData = true);
    void setEnclosure(bool refreshData = true);
    void setSkipRows(bool refreshData = true);
    void setHeaderOption(bool refreshData = true);
    void setBackslashAsEscape(bool refreshData = true);
    void setNullIf(bool refreshData = true);
    
private:
    QComboBox *fileEncodingComboBox;
    QComboBox *delimiterComboBox;
    QSpinBox *skipRowsSpinBox;
    QComboBox *headerComboBox;
    QLineEdit *enclosureEditor;
    QCheckBox *backslashAsEscapeCheckBox;
    QLineEdit *nullIfEditor;

    DataTable *previewTable;
    QStandardItemModel *tableModel;

    CsvImporter importer;

    void createForm(QVBoxLayout *mainLayout);

    void previewData();

    QStringList createColumnNames(int count) const;
    
};

#endif // DATAIMPORTERCSVOPTIONSPAGE_H
