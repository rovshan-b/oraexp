#include "partitioningparamsdelegate.h"
#include "connection_page/table_creator/tablecreatortabs.h"
#include "models/tableindexesmodel.h"

PartitioningParamsDelegate::PartitioningParamsDelegate(IQueryScheduler *queryScheduler,
                                                       bool configureForIndex,
                                                       TableCreatorTabs *tableCreator,
                                                       QObject *parent) :
    AbstractTemplatedParamsDelegate<TablePartitioningInfo, PartitioningParamsDialog>(queryScheduler, parent),
    configureForIndex(configureForIndex), tableCreator(tableCreator)
{

}

PartitioningParamsDialog *PartitioningParamsDelegate::createEditorDialog(bool isEditMode, QWidget *parentWindow){

    PartitioningParamsDialog *dialog=new PartitioningParamsDialog(parentWindow,
                                                                  isEditMode ? DbObjectCreator::EditExisting : DbObjectCreator::CreateNew,
                                                                  configureForIndex,
                                                                  tableCreator);
    return dialog;
}

QString PartitioningParamsDelegate::generateDdl(const TablePartitioningInfo &params)
{
    return params.generateDdl(tableCreator->getTableType());
}

QString PartitioningParamsDelegate::generateDiffDdl(const TablePartitioningInfo &params, const TablePartitioningInfo &originalParams)
{
    QString ddl;

    QString schemaName;
    QString objectName;
    if(configureForIndex){
        schemaName=tableModel->data(tableModel->index(currentRowIx, TableIndexesModel::IndexOwner)).toString();
        objectName=tableModel->data(tableModel->index(currentRowIx, TableIndexesModel::IndexName)).toString();
    }else{
        schemaName=tableCreator->getSchemaName();
        objectName=tableCreator->getTableName();
    }

    QList< NameQueryPair> queries=params.generateAlterDdl(originalParams, schemaName, objectName, configureForIndex);
    foreach(NameQueryPair pair, queries)
    {
        ddl.append(pair.second).append(";\n");
    }
    return ddl;
}
