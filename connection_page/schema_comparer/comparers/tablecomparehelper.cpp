#include "tablecomparehelper.h"
#include "dbobjectscomparehelper.h"
#include "metadata_loaders/table/tableinfoloader.h"
#include "code_generators/ddl/tableddlgenerator.h"
#include <QDebug>

TableCompareHelper::TableCompareHelper(const QString &compareResult, const QString &currentObjectName, QObject *parent) :
    DbObjectCompareHelper(DbTreeModel::Tables, compareResult, currentObjectName, parent)
{
}

TableCompareHelper::~TableCompareHelper()
{
    qDebug("deleted TableCompareHelper");
}

QPair<QString, QString> TableCompareHelper::generateCreateDdl(DbObjectInfo *objectInfo, const SchemaComparisonOptions *options)
{
    TableInfo *sourceTableInfo=dynamic_cast<TableInfo*>(objectInfo);
    Q_ASSERT(sourceTableInfo);
    if(sourceTableInfo->generalInfo.schema!=dbObjectsCompareHelper->getTargetSchemaName()){
        sourceTableInfo->setSchemaName(dbObjectsCompareHelper->getTargetSchemaName());
    }
    sourceTableInfo->prepareForOptions(options->tableCreateOptions.newObjectOptions);

    return TableDdlGenerator::generateDdlForExporting(*sourceTableInfo, options->tableCreateOptions);
}

QPair<QString, QString> TableCompareHelper::generateDiffDdl(DbObjectInfo *srcObjectInfo, DbObjectInfo *targetObjectInfo, const SchemaComparisonOptions *options)
{
    TableInfo *sourceTableInfo=static_cast<TableInfo*>(srcObjectInfo);
    TableInfo *targetTableInfo=static_cast<TableInfo*>(targetObjectInfo);

    Q_ASSERT(sourceTableInfo);
    Q_ASSERT(targetObjectInfo);

    if(sourceTableInfo->generalInfo.schema!=targetTableInfo->generalInfo.schema){
        sourceTableInfo->setSchemaName(targetTableInfo->generalInfo.schema);
    }

    if(sourceTableInfo->needsRecreation(*targetTableInfo, options->tableDiffOptions.partitions)){
        sourceTableInfo->prepareForOptions(options->tableCreateOptions.newObjectOptions);
        QString recreateDdl=TableDdlGenerator::generateDropDdl(*targetTableInfo);
        QPair<QString,QString> ddlPair = TableDdlGenerator::generateDdlForExporting(*sourceTableInfo, options->tableCreateOptions);
        recreateDdl.append("\n").append(ddlPair.first);

        ddlPair.first=recreateDdl;

        return ddlPair;
    }else{
        targetTableInfo->prepareForOptions(options->tableDiffOptions.newObjectOptions); //if we start to generate MOVE statement for tablespaces will need to change/remove this line
        return TableDdlGenerator::generateDiffDdl(*sourceTableInfo, *targetTableInfo, options->tableDiffOptions);
    }
}
