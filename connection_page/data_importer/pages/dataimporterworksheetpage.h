#ifndef DATAIMPORTERWORKSHEETPAGE_H
#define DATAIMPORTERWORKSHEETPAGE_H

#include "connection_page/connectionpagewizardpage.h"

class WorksheetWidget;
class IQueryScheduler;

class DataImporterWorksheetPage : public ConnectionPageWizardPage
{
    Q_OBJECT
public:
    explicit DataImporterWorksheetPage(DbUiManager *uiManager, QWidget *parent = 0);

    void setQueryScheduler(IQueryScheduler *queryScheduler);

    virtual void initializePage();

private:
    WorksheetWidget *worksheetWidget;
    
};

#endif // DATAIMPORTERWORKSHEETPAGE_H
