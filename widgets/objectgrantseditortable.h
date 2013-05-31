#ifndef OBJECTGRANTSEDITORTABLE_H
#define OBJECTGRANTSEDITORTABLE_H

#include "datatableandtoolbarwidget.h"
#include "beans/objectgrantinfo.h"
#include "navtree/dbtreemodel.h"
#include "models/objectgrantsmodel.h"
#include "connection_page/db_object_creator/dbobjectcreator.h"

class ObjectGrantsModel;
class SchemaSelectorDelegate;

class ObjectGrantsEditorTable : public DataTableAndToolBarWidget
{
    Q_OBJECT
public:
    explicit ObjectGrantsEditorTable(DbObjectCreator::CreatorMode creatorMode,
                                     OraExp::GrantsEditorPerspective perspective,
                                     DbTreeModel::DbTreeNodeType objectType,
                                     const QString &initialSchemaName ,
                                     QWidget *parent = 0);

    void setQueryScheduler(IQueryScheduler *queryScheduler);

    QList<ObjectGrantInfo> getList() const;
    void populateTable(QList<ObjectGrantInfo> *originalItemList);

    void removeIncorrectRows();

    void setInitialUserOrObjectName(const QString &initialSchemaName);

    ObjectGrantsModel *model() const {return static_cast<ObjectGrantsModel*>(table()->model());}

signals:
    void ddlChanged();

protected slots:
    void alterQuerySucceeded(const QString &taskName);
    void alterQueryError(const QString &taskName);
    void tableDataChanged(const QModelIndex &from, const QModelIndex &to);

private:
    IQueryScheduler *queryScheduler;

    DbObjectCreator::CreatorMode creatorMode;
    OraExp::GrantsEditorPerspective perspective;
    DbTreeModel::DbTreeNodeType objectType;
    QString initialSchemaName;

    QList<ObjectGrantInfo> *originalItemList;

    SchemaSelectorDelegate *schemaListDelegate;

    void customizeTable();
    static void setRowData(int rowIx, ObjectGrantsModel *model, ObjectGrantInfo *info);
    
};

#endif // OBJECTGRANTSEDITORTABLE_H
