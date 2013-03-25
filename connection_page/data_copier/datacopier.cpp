#include "datacopier.h"
#include "tabs/datacopieroptionstab.h"

DataCopier::DataCopier(DbUiManager *uiManager, QWidget *parent) :
    DataComparer(uiManager, parent)
{
}

DbObjectComparerOptionsTab *DataCopier::doCreateOptionsTab()
{
    return new DataCopierOptionsTab();
}
