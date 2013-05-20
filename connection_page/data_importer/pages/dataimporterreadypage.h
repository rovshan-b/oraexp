#ifndef DATAIMPORTERREADYPAGE_H
#define DATAIMPORTERREADYPAGE_H

#include "connection_page/connectionpagewizardpage.h"

class QLabel;

class DataImporterReadyPage : public ConnectionPageWizardPage
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
