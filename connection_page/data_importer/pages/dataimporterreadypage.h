#ifndef DATAIMPORTERREADYPAGE_H
#define DATAIMPORTERREADYPAGE_H

#include <QWizardPage>

class QLabel;

class DataImporterReadyPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit DataImporterReadyPage(QWidget *parent = 0);
    
    void initializePage();

private:
    QLabel *schemaNameLabel;
    QLabel *tableNameLabel;
    QLabel *fileNameLabel;
    QLabel *beforeImportQueryLabel;
    QLabel *importQueryLabel;
    QLabel *afterImportQueryLabel;
    
};

#endif // DATAIMPORTERREADYPAGE_H
