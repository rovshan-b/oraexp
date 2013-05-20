#ifndef DATAIMPORTERWORKSHEETPAGE_H
#define DATAIMPORTERWORKSHEETPAGE_H

#include "connection_page/connectionpagewizardpage.h"

class WorksheetWidget;
class IQueryScheduler;

class DataImporterWorksheetPage : public ConnectionPageWizardPage
{
    Q_OBJECT
public:
    explicit DataImporterWorksheetPage(QWidget *parent = 0);

    void setQueryScheduler(IQueryScheduler *queryScheduler);

private:
    WorksheetWidget *worksheetWidget;
    
};

#endif // DATAIMPORTERWORKSHEETPAGE_H
