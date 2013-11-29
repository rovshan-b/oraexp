#include "dataimporterworksheetpage.h"
#include "connection_page/worksheet/worksheetwidget.h"
#include "code_generators/dml/tabledmlgenerator.h"
#include <QtGui>

DataImporterWorksheetPage::DataImporterWorksheetPage(DbUiManager *uiManager, QWidget *parent) :
    ConnectionPageWizardPage(uiManager, parent)
{
    setTitle(tr("Check results"));
    setSubTitle(tr("Check data import results and COMMIT/ROLLBACK as necessary"));

    QVBoxLayout *mainLayout = new QVBoxLayout();

    worksheetWidget = new WorksheetWidget(uiManager);
    mainLayout->addWidget(worksheetWidget);

    setLayout(mainLayout);
}

void DataImporterWorksheetPage::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    worksheetWidget->setQueryScheduler(queryScheduler);
}

void DataImporterWorksheetPage::initializePage()
{
    worksheetWidget->setContents(TableDmlGenerator::generateSelectStatement(field("schemaName").toString().trimmed().toUpper(),
                                                                            field("tableName").toString().trimmed().toUpper()));
}
