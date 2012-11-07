#ifndef OBJECTGRANTSEDITORTABLE_H
#define OBJECTGRANTSEDITORTABLE_H

#include "datatableandtoolbarwidget.h"
#include "beans/objectgrantinfo.h"
#include "navtree/dbtreemodel.h"
#include "models/objectgrantsmodel.h"

class ObjectGrantsModel;
class SchemaSelectorDelegate;

class ObjectGrantsEditorTable : public DataTableAndToolBarWidget
{
    Q_OBJECT
public:
    explicit ObjectGrantsEditorTable(bool editMode,
                                     OraExp::GrantsEditorPerspective perspective,
                                     DbTreeModel::DbTreeNodeType objectType,
                                     const QString &initialUserOrObjectName ,
                                     QWidget *parent = 0);

    void setQueryScheduler(IQueryScheduler *queryScheduler);

    QList<ObjectGrantInfo> getList() const;
    void populateTable(QList<ObjectGrantInfo> *originalItemList);

    void removeIncorrectRows();

    void setInitialUserOrObjectName(const QString &initialUserOrObjectName);

signals:
    void ddlChanged();

protected slots:
    void alterQuerySucceeded(const QString &taskName);
    void alterQueryError(const QString &taskName);
    void tableDataChanged(const QModelIndex &from, const QModelIndex &to);

private:
    IQueryScheduler *queryScheduler;

    bool editMode;
    OraExp::GrantsEditorPerspective perspective;
    DbTreeModel::DbTreeNodeType objectType;
    QString initialUserOrObjectName;

    QList<ObjectGrantInfo> *originalItemList;

    SchemaSelectorDelegate *schemaListDelegate;

    void customizeTable();
    static void setRowData(int rowIx, ObjectGrantsModel *model, ObjectGrantInfo *info);
    
};

#endif // OBJECTGRANTSEDITORTABLE_H
