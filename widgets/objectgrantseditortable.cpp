#include "objectgrantseditortable.h"
#include "delegates/indexbasedcomboboxdelegate.h"
#include "delegates/schemaselectordelegate.h"
#include "connectivity/dbconnection.h"
#include "util/iconutil.h"
#include "util/dbutil.h"
#include "util/itemcreatorhelper.h"
#include "util/widgethelper.h"

ObjectGrantsEditorTable::ObjectGrantsEditorTable(bool editMode,
                                                 OraExp::GrantsEditorPerspective perspective,
                                                 DbTreeModel::DbTreeNodeType objectType,
                                                 const QString &initialUserOrObjectName,
                                                 QWidget *parent) :
    DataTableAndToolBarWidget(0, Qt::Horizontal, parent),
    editMode(editMode),
    perspective(perspective),
    objectType(objectType),
    initialUserOrObjectName(initialUserOrObjectName),
    originalItemList(0),
    schemaListDelegate(0)
{
}

void ObjectGrantsEditorTable::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    this->queryScheduler=queryScheduler;

    customizeTable();
}

QList<ObjectGrantInfo> ObjectGrantsEditorTable::getList() const
{
    return ItemCreatorHelper::getItemList<ObjectGrantInfo,ObjectGrantsModel>(
                static_cast<ObjectGrantsModel*>(table()->model()));
}

void ObjectGrantsEditorTable::populateTable(QList<ObjectGrantInfo> *originalItemList)
{
    Q_ASSERT(this->originalItemList==0);

    this->originalItemList=originalItemList;
    if(this->originalItemList->size()>0){
        ItemCreatorHelper::populateTableWithItems<ObjectGrantInfo,ObjectGrantsModel>(originalItemList,
                                                                                 this->table(),
                                                                                 &ObjectGrantsEditorTable::setRowData);
    }
}

void ObjectGrantsEditorTable::removeIncorrectRows()
{
    WidgetHelper::removeIncorrectRows(table());
}

void ObjectGrantsEditorTable::setInitialUserOrObjectName(const QString &initialUserOrObjectName)
{
    this->initialUserOrObjectName=initialUserOrObjectName;
    if(schemaListDelegate!=0){
        schemaListDelegate->setInitialValue(initialUserOrObjectName);
    }
}

void ObjectGrantsEditorTable::alterQuerySucceeded(const QString &taskName)
{
    Q_ASSERT(originalItemList);

    int rowNum=numberAfterLastUnderscore(taskName);
    Q_ASSERT(rowNum>0);

    DataTable *tbl=table();

    ObjectGrantsModel *model=static_cast<ObjectGrantsModel*>(tbl->model());
    int rowIx=rowNum-1;

    //if(!taskName.startsWith("add_")){
    //    Q_ASSERT(originalItemList->size()>rowIx);
    //}

    ObjectGrantInfo modifiedGrantInfo=model->itemInfoFromModelRow(rowIx);

    if(taskName.startsWith("add_tbl_grant_")){

        if(rowIx>=originalItemList->size()){
            originalItemList->append(modifiedGrantInfo);
            model->freezeRow(rowIx);
            model->setColumnEnabled(ObjectGrantsModel::GrantSchemaOrObject, false, rowIx);
        }

        int lastUnderscoreIx=taskName.lastIndexOf('_');
        int prevUnderscoreIx=taskName.lastIndexOf('_', lastUnderscoreIx-1);
        QString privilegeName=taskName.mid(prevUnderscoreIx+1, lastUnderscoreIx-prevUnderscoreIx-1);
        qDebug() << privilegeName;

        QStringList privilegeNames;
        DbUtil::populatePrivilegeNames(privilegeNames, this->objectType);
        int colIx=privilegeNames.indexOf(privilegeName)+1;
        Q_ASSERT(colIx>0);

        (*originalItemList)[rowIx]=modifiedGrantInfo;

    }else if(taskName.startsWith("revoke_all_privileges_")){

        (*originalItemList)[rowIx].dropped=true;

    }else if(taskName.startsWith("change_grant_type_")){

        (*originalItemList)[rowIx]=modifiedGrantInfo;

    }

    ItemCreatorHelper::markDataChanges(model, rowIx, originalItemList);
}

void ObjectGrantsEditorTable::alterQueryError(const QString &taskName)
{
}

void ObjectGrantsEditorTable::tableDataChanged(const QModelIndex &from, const QModelIndex &to)
{
    ObjectGrantsModel *model=static_cast<ObjectGrantsModel*>(table()->model());
    ItemCreatorHelper::handleTableDataChanged(model, originalItemList, from, to);
}

void ObjectGrantsEditorTable::customizeTable()
{
    QStringList columnNames;
    columnNames.append(this->perspective==OraExp::ObjectGrants ? tr("User") : tr("Object"));

    QStringList priviligeNames;
    DbUtil::populatePrivilegeNames(priviligeNames, this->objectType);

    columnNames.append(priviligeNames);

    ObjectGrantsModel *tblModel=new ObjectGrantsModel(columnNames,
                                                        this->perspective,
                                                        priviligeNames,
                                                        this->objectType,
                                                        this);
    DataTable *tbl=table();
    setModel(tblModel);

    tbl->horizontalHeader()->setDefaultSectionSize(150);
    tbl->setEditTriggers(QAbstractItemView::AllEditTriggers);

    if(this->perspective==OraExp::ObjectGrants){
        schemaListDelegate=new SchemaSelectorDelegate(initialUserOrObjectName, this->queryScheduler, this, true);
        tbl->setItemDelegateForColumn(ObjectGrantsModel::GrantSchemaOrObject, schemaListDelegate);
    }else{
        DbItemListDelegate *objectListDelegate=new DbItemListDelegate(initialUserOrObjectName, this->queryScheduler,
                                                                      "get_tablespace_list", "tbl", this, true);
        tbl->setItemDelegateForColumn(ObjectGrantsModel::GrantSchemaOrObject, objectListDelegate);
    }

    QStringList grantTypes;
    grantTypes.append("");
    grantTypes.append(tr("GRANT"));
    grantTypes.append(tr("WITH GRANT OPTION"));

    QIcon grantIcon=QIcon();

    for(int i=0; i<priviligeNames.size(); ++i){
        IndexBasedComboBoxDelegate *privDelegate=new IndexBasedComboBoxDelegate(grantIcon, grantTypes, this);
        tbl->setItemDelegateForColumn(i+1, privDelegate); //first column is user or object name
    }

    if(this->editMode){
        connect(tblModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(tableDataChanged(QModelIndex,QModelIndex)));
    }

    connect(tblModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SIGNAL(ddlChanged()));
}

void ObjectGrantsEditorTable::setRowData(int rowIx, ObjectGrantsModel *model, ObjectGrantInfo *info)
{
    model->setData(model->index(rowIx, ObjectGrantsModel::GrantSchemaOrObject),
                   model->getPerspective()==OraExp::ObjectGrants ? info->grantee : info->objectName);

    QStringList privNames=model->getPrivilegeNames();
    for(int k=0; k<privNames.size(); ++k){
        model->setData(model->index(rowIx, k+1 /*1st column is schema name*/), info->privileges.value(privNames.at(k), 0), Qt::EditRole);
    }
}
