#include "dataimporterworksheetpage.h"
#include "connection_page/worksheet/worksheetwidget.h"
#include <QtGui>

DataImporterWorksheetPage::DataImporterWorksheetPage(QWidget *parent) :
    QWizardPage(parent)
{
    setTitle(tr("Check results"));
    setSubTitle(tr("Check data import results and COMMIT/ROLLBACK as necessary"));

    QVBoxLayout *mainLayout = new QVBoxLayout();

    worksheetWidget = new WorksheetWidget();
    mainLayout->addWidget(worksheetWidget);

    setLayout(mainLayout);
}

void DataImporterWorksheetPage::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    worksheetWidget->setQueryScheduler(queryScheduler);
}
