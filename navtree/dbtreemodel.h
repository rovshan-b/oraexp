#ifndef DBTREEMODEL_H
#define DBTREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include "dbtreeitemresult.h"

class IQueryScheduler;
class DbConnection;
class DbUiManager;
class DbSchemaItem;
class NodePopulatorThread;
class IDbTreeItemCheckStateProvider;

class DbTreeModel : public QAbstractItemModel
{
    Q_OBJECT
    Q_ENUMS(DbTreeNodeType)
public:

    enum DbTreeNodeType
    {
        //first level
        Tables,
        Views,
        Packages,
        PackageSpecs,
        PackageBodies,
        Procedures,
        Functions,
        SchemaTriggers,
        SchemaIndexes,
        Sequences,
        Types,
        TypeSpecs,
        TypeBodies,
        Synonyms,
        PublicSynonyms,
        DatabaseLinks,
        PublicDatabaseLinks,
        AllSchemas,

        //second level
        Table,
        View,
        Package,
        Procedure,
        Function,
        Sequence,
        Type,
        Synonym,
        DatabaseLink,
        Schema,

        //third level
        Columns,
        Constraints,
        Indexes,
        Triggers,
        PackageSpec,
        PackageBody,
        TypeSpec,
        TypeBody,
        IndexColumn,

        //fourth level
        Column,
        Constraint,
        Index,
        Trigger,

        //all
        Loading,
        All,
        Unknown

    };
    class DbTreeNodeTypes : public QList<DbTreeModel::DbTreeNodeType>
    {
    public:
        bool testFlag(DbTreeNodeType nodeType) const
        {
            if(isEmpty() || contains(All) || contains(nodeType)){
                return true;
            }

            return false;
        }

        //static DbTreeNodeTypes getFirstLevelItems() const
        //{
        //    return DbTreeNodeTypes() <<
        //}
    };

    static QString getDbTreeNodeName(DbTreeNodeType nodeType);
    static DbTreeNodeType getDbTreeNodeType(const QString &nodeTypeName);


    explicit DbTreeModel(IQueryScheduler *m_queryScheduler, const QString &defaultSchemaName, QObject *parent = 0);
    ~DbTreeModel();

    QVariant data(const QModelIndex &index, int role) const;
    bool setData (const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    bool hasChildren(const QModelIndex &parent) const;
    bool canFetchMore(const QModelIndex &parent) const;
    void fetchMore (const QModelIndex & parent);
    void loadChildItems(const QModelIndex &parent);

    IQueryScheduler *getQueryScheduler() const {return this->m_queryScheduler;}
    DbConnection *getDb() const;
    void setUiManager(DbUiManager *m_uiManager);
    DbUiManager *getUiManager() const;

    void refreshChildren(const QModelIndex & parent);

    void setupInitialItems(DbTreeNodeTypes nodeTypesToDisplay=DbTreeNodeTypes());

    bool checkboxesEnabled() const {return this->m_displayCheckBoxes;}
    void enableCheckboxes(bool enable=true) {this->m_displayCheckBoxes=enable;}
    bool checkItem(const QModelIndex &index, Qt::CheckState newCheckState, bool updateParentItem);
    void updateItemCheckedState(const QModelIndex &index, bool emitChangedSignalForChildren=false);
    void updateParentCheckedState(const QModelIndex &index);

    void setCheckableItemTypes(DbTreeNodeTypes checkableItemTypes){this->m_checkableItemTypes=checkableItemTypes;}

    void checkAll(const QModelIndex &parent, bool check=true);

    QList<QModelIndex> getCheckedGrandChildren(const QModelIndex &grandfather, DbTreeNodeType parentNodeType) const;

    QList<DbTreeItem*> getChildItems(const QModelIndex &parent, bool checkType=false, DbTreeNodeType nodeType=DbTreeModel::All) const;
    QList<QModelIndex> getChildIndexes(const QModelIndex &parent, DbTreeNodeType nodeType) const;
    QModelIndex getChildIndex(const QModelIndex &parent, DbTreeNodeType nodeType) const;

    bool onlyTopLevelCheckable() const {return m_onlyTopLevelCheckable;}
    void setOnlyTopLevelCheckable(bool checkable) {m_onlyTopLevelCheckable=checkable;}

    void setCheckStateProvider(IDbTreeItemCheckStateProvider *provider);
    bool isCheckable(const QModelIndex &index) const;

    QString itemName(const QModelIndex &index) const;

    QString getDefaultSchemaName() const;

    QModelIndex findByName(const QModelIndex &parent, const QString &name) const;

    int getCheckedChildCount(const QModelIndex &parent);
    QStringList getCheckedChildNames(const QModelIndex &parent);
signals:
    void childrenPopulated(const QModelIndex &parent);
    void childrenPopulateError(const QModelIndex &parent, const OciException &exception);

private slots:
    void nodeListAvailable(const DbTreeItemResult &result);

private:
    bool isValidIndex(const QModelIndex &index) const;
    void populateChildNodes(const QModelIndex & parent);
    void clearChildren(const QModelIndex & parent, bool resetPopulatedFlag=true);

    void startNodePopulatorThread(const QModelIndex &parent);
    void deleteNodePopulatorThread();

    DbSchemaItem *m_rootItem;
    IQueryScheduler *m_queryScheduler;
    DbUiManager *m_uiManager;

    NodePopulatorThread *m_nodePopulatorThread;

    volatile bool m_fetchInProgress;

    bool m_displayCheckBoxes;
    DbTreeNodeTypes m_checkableItemTypes;
    bool m_onlyTopLevelCheckable;

    bool isCheckable(DbTreeItem *item) const;

    IDbTreeItemCheckStateProvider *m_checkStateProvider;
};

#endif // DBTREEMODEL_H
