#ifndef DBOBJECTCOMPARERCOMPARETAB_H
#define DBOBJECTCOMPARERCOMPARETAB_H

#include <QWidget>
#include "util/queryscheduler.h"
#include "navtree/dbtreemodel.h"

class QGroupBox;
class QCheckBox;
class DbTreeView;
class QBoxLayout;
class DbItemListComboBox;
class ConnectionSelectorWidget;
class DbConnection;
class DbUiManager;
class QPushButton;

class DbObjectComparerCompareTab : public QWidget, public QueryScheduler
{
    Q_OBJECT
public:
    explicit DbObjectComparerCompareTab(DbUiManager *uiManager, QWidget *parent = 0);

    void setQueryScheduler(IQueryScheduler *queryScheduler);

    virtual void createUi();

    virtual DbConnection *getDb() const;
    virtual QObject *getQueryEndMonitor();

    QString getSourceSchemaName() const;
    QString getTargetSchemaName() const;
    DbTreeModel *getObjectsModel() const;

    virtual void beforeCompare();

signals:
    void uiCreated();
    void connectionEstablished(DbConnection *db);

private slots:
    void loadDbObjects();
    virtual void treeChildrenPopulated(const QModelIndex &parent);
    virtual void currentTreeItemChanged(const QModelIndex & current, const QModelIndex & previous);
    void targetConnectionEstablished(DbConnection *db);
    void targetSchemaListLoaded();
    void loadSchemaObjects();
    void selectAll();
    void selectNone();

protected:
    virtual void beforeEnqueueQuery();
    virtual DbTreeModel::DbTreeNodeTypes getNodeTypesToDisplay() const;
    virtual DbTreeModel::DbTreeNodeTypes getCheckableNodeTypes() const;

    DbUiManager *uiManager;
    QObject *parentQueryEndMonitor;
    IQueryScheduler *queryScheduler;

    DbItemListComboBox *sourceSchemaComboBox;
    ConnectionSelectorWidget *targetConnection;
    DbItemListComboBox *targetSchemaComboBox;

    DbTreeView *schemaObjectsTree;

    QPushButton *btnSelectAll;
    QPushButton *btnSelectNone;

    void createConnectionOptionsPane(QBoxLayout *layout);
    virtual void createItemsTable(QBoxLayout *layout);
    void createActionButtons(QBoxLayout *layout);
    
};

#endif // DBOBJECTCOMPARERCOMPARETAB_H
