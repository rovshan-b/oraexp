#ifndef DATAIMPORTERWORKSHEETPAGE_H
#define DATAIMPORTERWORKSHEETPAGE_H

#include <QWizardPage>

class WorksheetWidget;
class IQueryScheduler;

class DataImporterWorksheetPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit DataImporterWorksheetPage(QWidget *parent = 0);

    void setQueryScheduler(IQueryScheduler *queryScheduler);
    
private:
    WorksheetWidget *worksheetWidget;
    
};

#endif // DATAIMPORTERWORKSHEETPAGE_H
