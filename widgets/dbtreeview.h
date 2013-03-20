#ifndef DBTREEVIEW_H
#define DBTREEVIEW_H

#include <QTreeView>
#include "navtree/dbtreemodel.h"

class IQueryScheduler;
class DbUiManager;

class DbTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit DbTreeView(QWidget *parent = 0);

    void setQueryScheduler(IQueryScheduler *queryScheduler, const QString &defaultSchemaName, DbUiManager *uiManager, bool enableHorizontalScrollbar=true);
    IQueryScheduler *getQueryScheduler() const;

    QString getDefaultSchemaName() const {return this->defaultSchemaName;}

    DbTreeModel *getModel() const;

    void checkAll(const QModelIndex &parent, bool check=true);

signals:
    void treeViewReady();

    void childrenPopulated(const QModelIndex &parent);
    void childrenPopulateError(const QModelIndex &parent, const OciException &exception);

private slots:
    void showContextMenu(const QPoint &);
    void handleChildrenPopulateError(const QModelIndex &parent, const OciException &exception);

private:
    QString defaultSchemaName;

};

#endif // DBTREEVIEW_H
