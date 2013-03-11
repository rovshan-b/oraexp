#include "tableadditionalattributes.h"
#include "util/strutil.h"

TableAdditionalAttributes::TableAdditionalAttributes() :
    parallel(0),
    parallelDegree(-1),
    logging(0),
    cache(0),
    monitoring(0),
    compress(0),
    rowDependencies(0),
    rowMovement(0),
    flashbackArchive(0)
{
}

QString TableAdditionalAttributes::generateDdl(OraExp::TableType tableType, bool isPartitioned, bool fbArchive) const
{
    QString ddl;

    if(tableType==OraExp::TableTypeTemporarySession ||
            tableType==OraExp::TableTypeTemporaryTransaction){
        return ddl;
    }

    ddl.append(optionText(parallel, "PARALLEL ", "NOPARALLEL "));
    if(parallel==1 && parallelDegree!=-1){
        ddl.append(QString::number(parallelDegree)).append(" ");
    }

    if(tableType!=OraExp::TableTypeIndexOrganized){
        ddl.append(optionText(logging, "LOGGING ", "NOLOGGING "));
    }

    if(tableType!=OraExp::TableTypeIndexOrganized){
        ddl.append(optionText(cache, "CACHE ", "NOCACHE "));
    }

    ddl.append(optionText(monitoring, "MONITORING ", "NOMONITORING "));

    if(tableType!=OraExp::TableTypeIndexOrganized){
        ddl.append(optionText(compress, "COMPRESS ", "NOCOMPRESS ", "COMPRESS FOR OLTP "));
    }

    ddl.append(optionText(rowDependencies, "ROWDEPENDENCIES ", "NOROWDEPENDENCIES "));

    if(tableType!=OraExp::TableTypeIndexOrganized ||
            (tableType==OraExp::TableTypeIndexOrganized && isPartitioned)){
        ddl.append(optionText(rowMovement, "ENABLE ROW MOVEMENT ", "DISABLE ROW MOVEMENT "));
    }

    if(fbArchive){
        if(flashbackArchive==1){
            ddl.append("FLASHBACK ARCHIVE ");
            if(!flashbackArchiveName.isEmpty()){
                ddl.append(flashbackArchiveName);
            }
        }else if(flashbackArchive==2){
            ddl.append("NO FLASHBACK ARCHIVE ");
        }
    }

    return ddl;
}

QString TableAdditionalAttributes::generateDiffDdl(const TableAdditionalAttributes &other, bool fbAchive) const
{
    QString ddl;

    if(parallel!=other.parallel){
        ddl.append(optionText(parallel, " PARALLEL", " NOPARALLEL"));
    }

    if(parallel==1 && parallelDegree!=-1 && parallelDegree!=other.parallelDegree){
        if(ddl.isEmpty()){
            ddl.append(" PARALLEL");
        }
        ddl.append(" ").append(QString::number(parallelDegree));
    }

    if(logging!=other.logging){
        ddl.append(optionText(logging, " LOGGING", " NOLOGGING"));
    }

    if(cache!=other.cache){
        ddl.append(optionText(cache, " CACHE", " NOCACHE"));
    }

    if(monitoring!=other.monitoring){
        ddl.append(optionText(monitoring, " MONITORING", " NOMONITORING"));
    }

    if(compress!=other.compress){
        ddl.append(optionText(compress, " COMPRESS", " NOCOMPRESS", " COMPRESS FOR OLTP"));
    }

    if(rowMovement!=other.rowMovement){
        ddl.append(optionText(rowMovement, " ENABLE ROW MOVEMENT", " DISABLE ROW MOVEMENT"));
    }

    if(fbAchive){
        if(flashbackArchive!=other.flashbackArchive){
            if(flashbackArchive==1){
                ddl.append("FLASHBACK ARCHIVE ");
                if(!flashbackArchiveName.isEmpty()){
                    ddl.append(flashbackArchiveName);
                }
            }else if(flashbackArchive==2){
                ddl.append("NO FLASHBACK ARCHIVE ");
            }
        }else if(flashbackArchive==other.flashbackArchive){
            if(flashbackArchiveName!=other.flashbackArchiveName){
                ddl.append("FLASHBACK ARCHIVE ").append(flashbackArchiveName.isEmpty() ? "DEFAULT" : flashbackArchiveName);
            }
        }
    }

    return ddl;
}

TableAdditionalAttributes TableAdditionalAttributes::fromFetchResult(const FetchResult &result)
{
    TableAdditionalAttributes a;

    QString degree=result.colValue("DEGREE").trimmed();
    a.parallel=(degree=="1" ? 2 : 1);
    a.parallelDegree=((degree=="1" || degree=="DEFAULT") ? -1 : degree.toInt());
    a.logging=result.colValue("LOGGING").trimmed()=="YES" ? 1 : 2;
    a.cache=result.colValue("CACHE").trimmed()=="Y" ? 1 : 2;
    a.monitoring=result.colValue("MONITORING")=="YES" ? 1 : 2;
    a.compress=result.colValue("COMPRESSION")=="ENABLED" ? 1 : 2;
    a.rowDependencies=result.colValue("DEPENDENCIES")=="ENABLED" ? 1 : 2;
    a.rowMovement=result.colValue("ROW_MOVEMENT")=="ENABLED" ? 1 : 2;

    if(result.columnTitles.contains("FLASHBACK_STATUS")){
        a.flashbackArchive=result.colValue("FLASHBACK_STATUS")=="ENABLED" ? 1 : 2;
        a.flashbackArchiveName=result.colValue("FLASHBACK_ARCHIVE_NAME");
    }

    return a;
}
