#include "datacopierresultstab.h"

DataCopierResultsTab::DataCopierResultsTab(DbUiManager *uiManager, QWidget *parent) :
    DataComparerResultsTab(uiManager, parent)
{
}

QStringList DataCopierResultsTab::getTableColumnNames() const
{
    return QStringList() <<
                            tr("Table name") <<
                            tr("Status") <<
                            tr("Inserts");
}
