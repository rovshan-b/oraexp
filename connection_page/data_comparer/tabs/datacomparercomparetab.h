#ifndef DATACOMPARERCOMPARETAB_H
#define DATACOMPARERCOMPARETAB_H

#include "connection_page/db_object_comparer/tabs/dbobjectcomparercomparetab.h"
#include "widgets/codeeditorandsearchpanewidget.h"
#include "../tableinfofordatacomparison.h"
#include "interfaces/isearchable.h"
#include "navtree/idbtreeitemcheckstateprovider.h"

class DataComparerCompareTab : public DbObjectComparerCompareTab, public ISearchable, public IDbTreeItemCheckStateProvider
{
    Q_OBJECT
public:
    DataComparerCompareTab(DbUiManager *uiManager, QWidget *parent);

    QHash<QString,TableInfoForDataComparison> getTableOptions() const;

    virtual bool isCheckable(DbTreeItem *item);

    virtual void beforeCompare();

protected:
    virtual void createItemsTable(QBoxLayout *layout);
    virtual DbTreeModel::DbTreeNodeTypes getNodeTypesToDisplay() const;
    virtual DbTreeModel::DbTreeNodeTypes getCheckableNodeTypes() const;

    void showSearchWidget(){whereClauseEditor->showSearchPane();}
    void findNext(){whereClauseEditor->findNext();}
    void findPrevious(){whereClauseEditor->findPrevious();}

private slots:
    virtual void treeChildrenPopulated(const QModelIndex &parent);
    virtual void currentTreeItemChanged(const QModelIndex & current, const QModelIndex & previous);

private:
    QWidget *rightPane;
    CodeEditorAndSearchPaneWidget *whereClauseEditor;
    QLineEdit *targetTablenameEditor;
    QHash<QString,TableInfoForDataComparison> tableOptions;

    QStringList supportedDataTypes;

    void commitWhereQuery(const QModelIndex &index);
};

#endif // DATACOMPARERCOMPARETAB_H
