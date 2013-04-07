#include "dataexporterresultstab.h"
#include <QStandardItemModel>

DataExporterResultsTab::DataExporterResultsTab(DbUiManager *uiManager, QWidget *parent) :
    DataComparerResultsTab(uiManager, parent)
{
}

QStringList DataExporterResultsTab::getTableColumnNames() const
{
    return QStringList() <<
                            tr("Table name") <<
                            tr("Status") <<
                            tr("Exported count");
}

void DataExporterResultsTab::updateDmlCounts(int row, const DataCompareInfo &info)
{
    if(info.inserts>0){
        tableModel->item(row, 2)->setText(QString::number(info.inserts));
    }
}
