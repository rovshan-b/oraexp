#include "dbobjecthasher.h"
#include "interfaces/iqueryscheduler.h"
#include <QStringList>
#include <QDebug>

DbObjectHasher::DbObjectHasher(QObject *parent) :
    QObject(parent)
{
}

void DbObjectHasher::createHashForObjects(IQueryScheduler *queryScheduler,
                                          const QString &objectType,
                                          const QString &schemaName,
                                          const QString &targetSchemaName /*hash will be compared with a hash in this target schema*/,
                                          SchemaComparisonOptions *options,
                                          const QStringList &objectNames,
                                          int startIx, int count, const QString &hashToCompareWith)
{
    QStringList subList=objectNames.mid(startIx, count);
    Q_ASSERT(subList.size());

    QString objectNamesStr=subList.join(",");
    qDebug() << "createHashForObjects ( " << objectType <<") =" << objectNamesStr;

    queryScheduler->enqueueQuery("create_object_hash_for_comparison",
                                 QList<Param*>()
                                 << new Param("owner", schemaName)
                                 << new Param("target_owner", targetSchemaName)
                                 << new Param("object_type", objectType)
                                 << new Param("object_names", objectNamesStr)
                                 << new Param("compare_hash", hashToCompareWith)
                                 << new Param("opt_tbl_comments", options->tableDiffOptions.comments)
                                 << new Param("opt_tbl_props", options->tableDiffOptions.properties)
                                 << new Param("opt_tbl_fb_archive", options->tableDiffOptions.flashbackArchive)
                                 << new Param("opt_tbl_storage", options->tableDiffOptions.storage)
                                 << new Param("opt_tbl_iot_props", options->tableDiffOptions.iotProperties)
                                 << new Param("opt_tbl_lob_props", options->tableDiffOptions.lobProperties)
                                 << new Param("opt_tbl_columns", options->tableDiffOptions.columns)
                                 << new Param("opt_tbl_constraints", options->tableDiffOptions.constraints)
                                 << new Param("opt_tbl_indexes", options->tableDiffOptions.indexes)
                                 << new Param("opt_tbl_parts", options->tableDiffOptions.partitions)
                                 << new Param("opt_tbl_ext_props", options->tableDiffOptions.externalProperties)
                                 << new Param("opt_tbl_triggers", options->tableDiffOptions.triggers)
                                 << new Param("opt_tbl_grants", options->tableDiffOptions.grants)
                                 << new Param("opt_src_ignore_whitespace", options->sourceCodeOptions.ignoreWhitespace)
                                 << new Param("opt_src_wrap", options->sourceCodeOptions.wrap)
                                 << new Param("opt_seq_check_currval", options->sequenceDiffOptions.updateCurrVal),
                                 this,
                                 "create_object_hash_for_comparison",
                                 "queryCompleted",
                                 "recordFetched",
                                 "fetchCompleted", true);
}

void DbObjectHasher::queryCompleted(const QueryResult &result)
{
    if(result.hasError){
        emit hashError(result.taskName, result.exception);
    }
}

void DbObjectHasher::recordFetched(const FetchResult &result)
{
    if(result.hasError){
        emit hashError(result.taskName, result.exception);
        //return;
    }else{
        qDebug() << "hashReady, hash =" << result.oneRow.at(0);
        emit hashReady(result.oneRow.at(0));
    }
}

void DbObjectHasher::fetchCompleted(const QString &)
{
}
