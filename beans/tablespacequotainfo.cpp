#include "tablespacequotainfo.h"
#include "util/dbutil.h"
#include "util/strutil.h"

TablespaceQuotaInfo::TablespaceQuotaInfo() :
    objId(-1),
    quota(0),
    quotaUnit(OraExp::Bytes),
    markedForDeletion(false),
    dropped(false)
{
}

bool TablespaceQuotaInfo::operator==(const TablespaceQuotaInfo &other) const
{
    return tablespaceName==other.tablespaceName &&
            quota==other.quota &&
            quotaUnit==other.quotaUnit &&
            unlimitedQuota==other.unlimitedQuota;
}

QString TablespaceQuotaInfo::generateDdl(const QString &username) const
{
    QString ddl;

    ddl.append("ALTER USER \"").append(username).append("\" ").
        append(getQuotaClause(unlimitedQuota, quota, quotaUnit));

    return ddl;
}

QString TablespaceQuotaInfo::generateDropDdl(const QString &username) const
{
    QString ddl;

    ddl.append("ALTER USER \"").append(username).append("\" ").
        append(getQuotaClause(false, 0, OraExp::Bytes));

    return ddl;
}

QList<NameQueryPair> TablespaceQuotaInfo::generateDiffDdl(const TablespaceQuotaInfo &other, const QString &username) const
{
    QList< NameQueryPair > result;

    if(other.dropped){
        return result;
    }

    QString ddl;

    if(markedForDeletion){
        ddl=generateDropDdl(username);
        result.append(qMakePair(QString("quota_drop_tablespace_quota_%1").arg(objId), ddl));
    }else if(! (*this == other)){
        ddl=generateDdl(username);
        result.append(qMakePair(QString("quota_alter_tablespace_quota_%1").arg(objId), ddl));
    }

    return result;
}

QString TablespaceQuotaInfo::generateListDdl(const QList<TablespaceQuotaInfo> *list,
                                             const QString &username)
{
    QString ddl;

    for(int i=0; i<list->size(); ++i){
        ddl.append("\n").append(list->at(i).generateDdl(username)).append(";");
    }

    return ddl;
}

QList<NameQueryPair> TablespaceQuotaInfo::generateListDiffDdl(const QList<TablespaceQuotaInfo> *currents,
                                                              const QList<TablespaceQuotaInfo> *others,
                                                              const QString &username)
{
    QList< NameQueryPair > result;

    Q_ASSERT(others->size()<=currents->size());

    int rowCount=currents->size();

    for(int i=0; i<rowCount; ++i){
        const TablespaceQuotaInfo &quotaInfo=currents->at(i);

        if(i<others->size()){
            const TablespaceQuotaInfo &originalQuotaInfo=others->at(i);
            QList< NameQueryPair > alterDdls=quotaInfo.generateDiffDdl(originalQuotaInfo, username);
            result.append(alterDdls);
        }else{
            result.append(qMakePair(QString("quota_add_tablespace_quota_%2").arg(i+1), quotaInfo.generateDdl(username)));
        }
    }

    return result;
}

bool TablespaceQuotaInfo::needsRecreation(const TablespaceQuotaInfo &) const
{
    return false;
}

TablespaceQuotaInfo TablespaceQuotaInfo::fromFetchResult(const FetchResult &result)
{
    TablespaceQuotaInfo info;

    info.tablespaceName=result.colValue("TABLESPACE_NAME");
    QString strBytes=result.colValue("MAX_BYTES");

    if(strBytes=="-1"){
        info.unlimitedQuota=true;
    }else{
        info.unlimitedQuota=false;
        //check if we can get it in gb
        qulonglong maxBytes=result.colValue("MAX_BYTES").toULongLong();
        if(maxBytes%(1024*1024*1024)==0){
            info.quota=maxBytes/(1024*1024*1024);
            info.quotaUnit=OraExp::GB;
        }else if(maxBytes%(1024*1024)==0){ //mb
            info.quota=maxBytes/(1024*1024);
            info.quotaUnit=OraExp::MB;
        }else if(maxBytes%(1024)==0){ //kb
            info.quota=maxBytes/(1024);
            info.quotaUnit=OraExp::KB;
        }else{
            info.quota=maxBytes;
            info.quotaUnit=OraExp::Bytes;
        }
    }

    return info;
}
/*
QString TablespaceQuotaInfo::toString() const
{
    return DbUtil::getExtentSizeClause(unlimitedQuota, quota, quotaUnit);
}*/

QString TablespaceQuotaInfo::getQuotaClause(bool unlimitedQuota, uint quota, OraExp::ExtentUnit quotaUnit) const
{
    QString ddl("QUOTA ");

    ddl.append(DbUtil::getExtentSizeClause(unlimitedQuota, quota, quotaUnit));
    ddl.append(" ON \"").append(tablespaceName).append("\"");

    return ddl;
}
