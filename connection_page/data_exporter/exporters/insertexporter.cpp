#include "insertexporter.h"
#include "beans/resultsetcolumnmetadata.h"
#include "util/strutil.h"
#include "util/dbutil.h"
#include "defines.h"

InsertExporter::InsertExporter()
{
}

void InsertExporter::startDocument(QTextStream & /*out*/)
{
    ResultsetColumnMetadata *metadata = columnMetadata.data();

    QStringList colNames;
    colNames.reserve(this->endColumn-this->startColumn+1);
    for(int i=this->startColumn; i<=this->endColumn; ++i){
        colNames.append(metadata->columnTitles.at(i));
    }

    insertStatement=QString("INSERT INTO ");

    if(!this->schemaName.isEmpty()){
        insertStatement.append(this->schemaName).append(".");
    }

    insertStatement.append(this->tableName).append(" (").append(joinEnclosed(colNames)).append(") VALUES (");

}



void InsertExporter::exportRow(const QStringList &oneRow, int rowIx, QTextStream &out)
{
    if(rowIx!=0){
        out << "\n";
    }

    out << insertStatement;

    for(int i=0; i<oneRow.size(); ++i){
        out << oneRow.at(i);

        if(i!=oneRow.size()-1){
            out << ",";
        }
    }

    out << ");";
}

void InsertExporter::prepareField(QString &fieldValue, int fieldIx)
{
    unsigned int colIx = this->startColumn + fieldIx + 1;

    OraExp::ColumnDataType dataType = columnMetadata->columnDataTypes.value(colIx);

    ResultsetColumnMetadata *metadata = columnMetadata.data();

    if(fieldValue.isEmpty()){
        fieldValue = "NULL";
    }else if(metadata->isTextColumn(colIx)){
        fieldValue.replace("'", "'||chr(39)||'").prepend("'").append("'");
    }else if(DbUtil::isNumericType(dataType)){
        //do nothing
    }else if(DbUtil::isDateType(dataType)){
        fieldValue = QString("to_date('%1', '%2')").arg(fieldValue, DB_DATE_FORMAT);
    }else if(DbUtil::isIntervalType(dataType)){
        fieldValue.prepend("'").append("'");
        fieldValue = DbUtil::toInterval(fieldValue, columnMetadata->columnSubTypes.value(colIx));
    }else if(DbUtil::isTimestampType(dataType)){
        fieldValue.prepend("'").append("'");
        fieldValue = DbUtil::toTimestamp(fieldValue, columnMetadata->columnSubTypes.value(colIx));
    }else{
        fieldValue.replace("'", "'||chr(39)||'").prepend("'").append("'");
    }
}
