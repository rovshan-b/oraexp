#ifndef DATAIMPORTERCSVOPTIONSPAGE_H
#define DATAIMPORTERCSVOPTIONSPAGE_H

#include <QWizardPage>

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
    
private:
    QComboBox *fileEncodingComboBox;
    QComboBox *delimiterComboBox;
    QSpinBox *skipRowsSpinBox;
    QComboBox *headerComboBox;
    QLineEdit *enclosureEditor;

    DataTable *previewTable;

    void createForm(QVBoxLayout *mainLayout);
    
};

#endif // DATAIMPORTERCSVOPTIONSPAGE_H
