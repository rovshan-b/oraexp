#include "objectresolverdialog.h"
#include "util/dialoghelper.h"
#include "connection_page/dbuimanager.h"
#include "mainwindow.h"
#include <QtGui>

ObjectResolverDialog::ObjectResolverDialog(const QString &objectName, DbUiManager *uiManager, QWidget *parent) :
    ConnectionPageWindow(uiManager, parent),
    objectName(objectName)
{
    DialogHelper::createProgressBarUi(this);

    connect(&resolver, SIGNAL(resolved(QString,DbTreeModel::DbTreeNodeType,QString,QString,ObjectResolver*)), this, SLOT(objectResolved(QString,DbTreeModel::DbTreeNodeType,QString,QString,ObjectResolver*)));
    connect(&resolver, SIGNAL(resolveError(OciException,ObjectResolver*)), this, SLOT(objectResolveError(OciException,ObjectResolver*)));

    QTimer::singleShot(1000, this, SLOT(makeVisibleIfInProgress()));
}

void ObjectResolverDialog::setConnection(DbConnection *db)
{
    ConnectionPageWindow::setConnection(db);

    setInProgress(true);

    resolver.resolveName(this->objectName, this);
}

void ObjectResolverDialog::objectResolved(const QString &objectTypeName, DbTreeModel::DbTreeNodeType objectType, const QString &schemaName, const QString &objectName, ObjectResolver *resolver)
{
    Q_UNUSED(objectTypeName)
    Q_UNUSED(resolver);

    setInProgress(false);

    uiManager->createViewer(schemaName, objectName, (int)objectType);

    accept();
}

void ObjectResolverDialog::objectResolveError(const OciException &ex, ObjectResolver *resolver)
{
    Q_UNUSED(ex);
    Q_UNUSED(resolver);

    setInProgress(false);

    hide();

    QMessageBox::critical(MainWindow::defaultInstance(), tr("Could not resolve"),
                          tr("Could not resolve name"));

    accept();
}
