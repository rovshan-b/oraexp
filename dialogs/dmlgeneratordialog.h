#ifndef DMLGENERATORDIALOG_H
#define DMLGENERATORDIALOG_H

#include "connection_page/connectionpagewindow.h"
#include "enums.h"

class DmlGeneratorDialog : public ConnectionPageWindow
{
    Q_OBJECT
public:
    explicit DmlGeneratorDialog(DbUiManager *uiManager,
                                const QString &schemaName,
                                const QString &tableName,
                                OraExp::DmlType dmlType,
                                QWidget *parent = 0);

    virtual void createUi();

    virtual void setConnection(DbConnection *db);
    
private:
    QString schemaName;
    QString tableName;
    OraExp::DmlType initialDmlType;
    
};

#endif // DMLGENERATORDIALOG_H
