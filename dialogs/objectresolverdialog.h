#ifndef OBJECTRESOLVERDIALOG_H
#define OBJECTRESOLVERDIALOG_H

#include "connection_page/connectionpagewindow.h"
#include "util/objectresolver.h"

class ObjectResolverDialog : public ConnectionPageWindow
{
    Q_OBJECT
public:
    explicit ObjectResolverDialog(const QString &objectName,
                                  DbUiManager *uiManager,
                                  QWidget *parent = 0);
    
    virtual void setConnection(DbConnection *db);

    virtual bool initiallyVisible() const{return false;}

private slots:
    void objectResolved(const QString &objectTypeName, DbTreeModel::DbTreeNodeType objectType, const QString &schemaName, const QString &objectName, ObjectResolver *resolver);
    void objectResolveError(const OciException &ex, ObjectResolver *resolver);

private:
    QString objectName;
    ObjectResolver resolver;
    
};

#endif // OBJECTRESOLVERDIALOG_H
