#ifndef DBOBJECTCOMPARERCOMPARETAB_H
#define DBOBJECTCOMPARERCOMPARETAB_H

#include <QWidget>
#include "util/queryscheduler.h"
#include "navtree/dbtreemodel.h"

class QGroupBox;
class QCheckBox;
class DbTreeViewPanel;
class QBoxLayout;
class QFormLayout;
class DbItemListComboBox;
class ConnectionSelectorWidget;
class DbConnection;
class DbUiManager;
class QPushButton;
class QTabWidget;
class ConnectionPageObject;

class DbObjectComparerCompareTab : public QWidget, public QueryScheduler
{
    Q_OBJECT
public:
    explicit DbObjectComparerCompareTab(DbUiManager *uiManager, QWidget *parent);

    void setQueryScheduler(IQueryScheduler *queryScheduler);

    virtual void createUi();

    virtual DbConnection *getDb() const;
    virtual QObject *getQueryEndMonitor();

    QString getSourceSchemaName() const;
    QString getTargetSchemaName() const;
    DbTreeModel *getObjectsModel() const;

    virtual void beforeCompare();

    virtual bool nestOptionsTab() const;
    virtual bool needsTargetConnection() const;

    QFormLayout *getTopPaneForm() const {return this->topPaneForm;}
    QTabWidget *getBottomPaneTab() const {return this->bottomPaneTab;}
    void addToBottomPaneTab(QWidget *widget, const QString &title);

    void setInitialSchemaAndObjectNames(const QString &initialSchemaName, const QString &initialObjectName);

signals:
    void uiCreated();
    void connectionEstablished(DbConnection *db);
    void sourceSchemaChanged(const QString &newSchemaName);
    void busyStateChanged(bool busy);

protected slots:
    void loadDbObjects();
    virtual void treeChildrenPopulated(const QModelIndex &parent);
    virtual void currentTreeItemChanged(const QModelIndex & current, const QModelIndex & previous);
    void targetConnectionEstablished(DbConnection *db);
    void targetSchemaListLoaded();
    void loadSchemaObjects();
    void sourceSchemaNameChanged();

protected:
    virtual void beforeEnqueueQuery();
    virtual DbTreeModel::DbTreeNodeTypes getNodeTypesToDisplay() const;
    virtual DbTreeModel::DbTreeNodeTypes getCheckableNodeTypes() const;

    DbUiManager *uiManager;
    ConnectionPageObject *parentTab;
    QObject *parentQueryEndMonitor;
    IQueryScheduler *queryScheduler;

    QFormLayout *topPaneForm;

    DbItemListComboBox *sourceSchemaComboBox;
    ConnectionSelectorWidget *targetConnection;
    DbItemListComboBox *targetSchemaComboBox;

    DbTreeViewPanel *schemaObjectsPanel;

    //QPushButton *btnSelectAll;
    //QPushButton *btnSelectNone;

    void createConnectionOptionsPane(QBoxLayout *layout);
    //virtual void addModuleSpecificControls(QFormLayout *layout) {Q_UNUSED(layout);}
    virtual void createItemsTable(QBoxLayout *layout);

    QTabWidget *bottomPaneTab;

    QString initialSchemaName;
    QString initialObjectName;
    
};

#endif // DBOBJECTCOMPARERCOMPARETAB_H
