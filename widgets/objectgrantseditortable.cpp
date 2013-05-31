#include "objectgrantseditortable.h"
#include "delegates/indexbasedcomboboxdelegate.h"
#include "delegates/schemaselectordelegate.h"
#include "delegates/grantobjectlistdelegate.h"
#include "connectivity/dbconnection.h"
#include "util/iconutil.h"
#include "util/dbutil.h"
#include "util/itemcreatorhelper.h"
#include "util/widgethelper.h"

ObjectGrantsEditorTable::ObjectGrantsEditorTable(DbObjectCreator::CreatorMode creatorMode,
                                                 OraExp::GrantsEditorPerspective perspective,
                                                 DbTreeModel::DbTreeNodeType objectType,
                                                 const QString &initialUserOrObjectName,
                                                 QWidget *parent) :
    DataTableAndToolBarWidget(0, Qt::Horizontal, parent),
    creatorMode(creatorMode),
    perspective(perspective),
    objectType(objectType),
    initialSchemaName(initialUserOrObjectName),
    originalItemList(0)
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
    this->initialSchemaName=initialUserOrObjectName;
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
            model->freezeRow(rowIx, true);
            //model->setColumnEnabled(ObjectGrantsModel::GrantSchema, false, rowIx);
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

void ObjectGrantsEditorTable::alterQueryError(const QString & /*taskName*/)
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
    columnNames.append(tr("User"));
    if(this->perspective==OraExp::UserGrants){
        columnNames.append(tr("Object"));
    }

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

    tbl->horizontalHeader()->setDefaultSectionSize(100);
    tbl->setColumnWidth(ObjectGrantsModel::GrantSchema, 170);
    tbl->setEditTriggers(QAbstractItemView::AllEditTriggers);


    schemaListDelegate=new SchemaSelectorDelegate(initialSchemaName, this->queryScheduler, this, true);
    tbl->setItemDelegateForColumn(ObjectGrantsModel::GrantSchema, schemaListDelegate);

    if(this->perspective==OraExp::UserGrants){
        GrantObjectListDelegate *objectListDelegate=new GrantObjectListDelegate(this->queryScheduler, ObjectGrantsModel::GrantSchema,
                                                                                this);
        tbl->setItemDelegateForColumn(ObjectGrantsModel::GrantObject, objectListDelegate);
        tbl->setColumnWidth(ObjectGrantsModel::GrantObject, 170);
        tblModel->setTitleColumn(ObjectGrantsModel::GrantObject);
    }

    QPixmap grantIcon = IconUtil::getIcon("grants");
    QStringList grantTypes = DbUtil::getGrantTypes();

    for(int i=0; i<priviligeNames.size(); ++i){
        int colIx = i+tblModel->offset();
        IndexBasedComboBoxDelegate *privDelegate=new IndexBasedComboBoxDelegate(this, colIx);
        tblModel->setList(colIx, grantTypes);
        tblModel->setColumnIcon(colIx, grantIcon);
        tbl->setItemDelegateForColumn(colIx, privDelegate);
    }

    if(creatorMode == DbObjectCreator::EditExisting){
        connect(tblModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(tableDataChanged(QModelIndex,QModelIndex)));

        tblModel->setColumnEnabled(ObjectGrantsModel::GrantSchema, false);

        if(this->perspective==OraExp::UserGrants){
            tblModel->setColumnEnabled(ObjectGrantsModel::GrantObject, false);
        }
    }

    connect(tblModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SIGNAL(ddlChanged()));
}

void ObjectGrantsEditorTable::setRowData(int rowIx, ObjectGrantsModel *model, ObjectGrantInfo *info)
{
    if(model->getPerspective()==OraExp::ObjectGrants){
        model->setData(model->index(rowIx, ObjectGrantsModel::GrantSchema), info->grantee);
    }else{
        QStringList parts=info->objectName.split("\".\"");
        if(parts.size()>=1){
            QString schema=parts.at(0);
            if(schema.startsWith("\"")){
                schema.remove(0,1);
            }
            model->setData(model->index(rowIx, ObjectGrantsModel::GrantSchema), schema);
        }
        if(parts.size()>=2){
            QString objectName=parts.at(1);
            if(objectName.endsWith("\"")){
                objectName.chop(1);
            }
            model->setData(model->index(rowIx, ObjectGrantsModel::GrantObject), objectName);
        }
    }

    QStringList privNames=model->getPrivilegeNames();
    for(int k=0; k<privNames.size(); ++k){
        model->setData(model->index(rowIx, k+model->offset()), info->privileges.value(privNames.at(k), 0), Qt::EditRole);
    }
}
