#include "dmlgeneratordialog.h"
#include "util/dialoghelper.h"


DmlGeneratorDialog::DmlGeneratorDialog(DbUiManager *uiManager,
                                       const QString &schemaName,
                                       const QString &tableName,
                                       OraExp::DmlType dmlType, QWidget *parent) :
    ConnectionPageWindow(uiManager, parent),
    schemaName(schemaName),
    tableName(tableName),
    initialDmlType(dmlType)
{

}

void DmlGeneratorDialog::createUi()
{
    //resize(this->sizeHint());
    //DialogHelper::centerWindow(this);
}

void DmlGeneratorDialog::setConnection(DbConnection *db)
{
    ConnectionPageWindow::setConnection(db);


}
