#include "bulkoperationhelper.h"
#include "util/dbutil.h"
#include "util/strutil.h"
#include "connectivity/statement.h"
#include "connectivity/resultset.h"
#include <QDebug>

//#include <iostream>
//using namespace std;

BulkOperationHelper::BulkOperationHelper() : bulkSize(-1), dmlMode(false)
{
}

BulkOperationHelper::~BulkOperationHelper()
{
    cleanup();
}

void BulkOperationHelper::setBulkSize(int size)
{
    this->bulkSize=size;
}

void BulkOperationHelper::createBufferForDataType(Statement *stmt, const QString &dataType, int length)
{
    Q_ASSERT(this->bulkSize>0);

    dataTypes.append(dataType);
    lenghts.append(length);

    void *result;


    if(DbUtil::isStringType(dataType) || DbUtil::isRawType(dataType)){

        Q_ASSERT(length>0);
        result = new dtext[this->bulkSize*(length+1)];
    }else if(DbUtil::isNumericType(dataType)){
        result = new double[this->bulkSize];
    }else if(DbUtil::isDateType(dataType)){
        result = stmt->createDateArray(this->bulkSize);
    }else if(DbUtil::isTimestampType(dataType)){
        result = stmt->createTimestampArray(dataType, this->bulkSize);
    }else if(DbUtil::isIntervalType(dataType)){
        result = stmt->createIntervalArray(dataType, this->bulkSize);
    }/*else if(DbUtil::isRawType(dataType)){
        Q_ASSERT(length>0);
        result = new dtext[this->bulkSize*(length)];
    }*/else{
        qDebug() << "Unsupported column data type for comparison:" << dataType;
        result = 0;
        Q_ASSERT(false);
    }

    Q_ASSERT(result);

    buffers.append(result);
}

/*
void BulkOperationHelper::printArrayData(Statement *stmt)
{
    wcout << "------------array data------------" << endl;
    for(int k=0; k<this->bulkSize; ++k){
        wcout << "row:" << endl;
        for(int i=0; i<dataTypes.size(); ++i){
            const QString &dataType=dataTypes.at(i);

            if(stmt->isBindNullAtPos(i+1, k+1)){
                wcout << L"\tNULL, ";
            }else if(DbUtil::isStringType(dataType)){

                dtext *currPosInBuffer = &((dtext*)buffers.at(i))[k*(lengthAt(i)+1)];
                wcout << L"\t" << currPosInBuffer << L", ";

            }else if(DbUtil::isNumericType(dataType)){
                wcout << L"\t" << ((double*)buffers.at(i))[k] << L", ";
            }else if(DbUtil::isDateType(dataType)){
                OCI_Date *date = ((OCI_Date**)buffers.at(i))[k];
                dtext str[21];
                OCI_DateToText(date, L"DD/MM/YYYY HH24:MI:SS", 20, str);
                wcout << L"\t" << str << L", ";
            }else{
                wcout << L"\tsomething else, ";
            }
        }
        wcout << endl;
    }
    wcout << "----------end array data----------" << endl;
}*/

QString BulkOperationHelper::dataTypeAt(int ix)
{
    return dataTypes.at(ix);
}

int BulkOperationHelper::lengthAt(int ix)
{
    return lenghts.at(ix);
}

void BulkOperationHelper::setDmlMode()
{
    this->dmlMode=true;
}

void BulkOperationHelper::bindArrays(Statement *stmt, const QString& bindVarPrefix, const QList<int> &bindVarSuffixes)
{
    Q_ASSERT(this->bulkSize>0);
    Q_ASSERT(bindVarSuffixes.isEmpty() || bindVarSuffixes.size()==buffers.size());

    for(int i=0; i<buffers.size(); ++i){
        QString bindVarName = QString(":%1_%2").arg(bindVarPrefix).arg(bindVarSuffixes.isEmpty() ? i : bindVarSuffixes.at(i));
        //qDebug() << "binding" << bindVarName;
        const QString &dataType=dataTypes.at(i);
        int length=lenghts.at(i);

        if(DbUtil::isStringType(dataType)){

            Q_ASSERT(length > 0);
            stmt->bindArrayOfStrings(bindVarName, (dtext*)buffers.at(i), length, dmlMode ? 0 : this->bulkSize);

        }else if(DbUtil::isNumericType(dataType)){
            stmt->bindArrayOfDoubles(bindVarName, (double*)buffers.at(i), dmlMode ? 0 : this->bulkSize);
        }else if(DbUtil::isDateType(dataType)){
            stmt->bindArrayOfDates(bindVarName, (OCI_Date**)buffers.at(i), dmlMode ? 0 : this->bulkSize);
        }else if(DbUtil::isTimestampType(dataType)){
            OraExp::ColumnSubType timestampSubType=DbUtil::getTimestampSubType(dataType);
            stmt->bindArrayOfTimestamps(bindVarName, (OCI_Timestamp**)buffers.at(i), DbUtil::toOciTimestampSubType(timestampSubType), dmlMode ? 0 : this->bulkSize);
        }else if(DbUtil::isIntervalType(dataType)){
            OraExp::ColumnSubType intervalSubType=DbUtil::getIntervalSubType(dataType);
            stmt->bindArrayOfIntervals(bindVarName, (OCI_Interval**)buffers.at(i), DbUtil::toOciIntervalSubType(intervalSubType), dmlMode ? 0 : this->bulkSize);
        }else if(DbUtil::isRawType(dataType)){
            Q_ASSERT(length > 0);
            stmt->bindArrayOfRaws(bindVarName, (dtext*)buffers.at(i), length, dmlMode ? 0 : this->bulkSize);
        }else{
            qDebug() << "Unsupported column data type for binding:" << dataType;
            Q_ASSERT(false);
        }
    }
}

void BulkOperationHelper::setArrayData(Statement *targetStmt, Resultset *sourceRs, int column, int pos)
{
    Q_ASSERT(this->bulkSize>0);

    if(sourceRs->isNull(column)){
        targetStmt->setBindNullAtPos(column, pos+1);
        nullifyStringAtPos(column-1, pos);
    }else{
        const QString &dataType=dataTypes.at(column-1);

        if(DbUtil::isStringType(dataType)){

            copyStringAtPos(column-1, pos, sourceRs->getString(column).toStdWString().c_str());

        }else if(DbUtil::isNumericType(dataType)){
            ((double*)buffers.at(column-1))[pos]=sourceRs->getDouble(column);//QString::number(sourceRs->getDouble(column)).toDouble();
        }else if(DbUtil::isDateType(dataType)){
            if(!OCI_DateAssign(((OCI_Date**)buffers.at(column-1))[pos], sourceRs->getDate(column))){
                throw OciException(QObject::tr("Failed to set date type value"));
            }
        }else if(DbUtil::isTimestampType(dataType)){
            if(!OCI_TimestampAssign(((OCI_Timestamp**)buffers.at(column-1))[pos], sourceRs->getTimestamp(column))){
                throw OciException(QObject::tr("Failed to set timestamp type value"));
            }
        }else if(DbUtil::isIntervalType(dataType)){
            if(!OCI_IntervalAssign(((OCI_Interval**)buffers.at(column-1))[pos], sourceRs->getInterval(column))){
                throw OciException(QObject::tr("Failed to set interval type value"));
            }
        }else if(DbUtil::isRawType(dataType)){
            dtext *buffer = (dtext*)buffers.at(column-1);
            dtext *currPosInBuffer = &buffer[pos*(lenghts.at(column-1)+1)];
            sourceRs->getRaw(column, currPosInBuffer, lenghts.at(column-1));
            currPosInBuffer[lenghts.at(column-1)] = L'\0';
        }else{
            qDebug() << "Unsupported column data type for populating array:" << dataType;
            Q_ASSERT(false);
        }
    }
}

void BulkOperationHelper::setArrayData(Statement *targetStmt, const QString &value, int column, int pos)
{
    if(value.isEmpty()){
        targetStmt->setBindNullAtPos(column, pos+1);
        nullifyStringAtPos(column-1, pos);
    }else{
        const QString &dataType=dataTypes.at(column-1);

        if(DbUtil::isNumericType(dataType)){
            bool conversionOk;
            double numVal = QString(value.trimmed()).replace(",", ".").toDouble(&conversionOk);
            if(conversionOk){
                ((double*)buffers.at(column-1))[pos] = numVal;
            }else{
                targetStmt->setBindNullAtPos(column, pos+1);
            }
        }else{ //try to set all other types as string

            copyStringAtPos(column-1, pos, value.toStdWString().c_str());

        }
    }
}

void BulkOperationHelper::nullifyArrayData(Statement *targetStmt, int offset)
{
    Q_ASSERT(this->bulkSize>0);

    for(int i=0; i<buffers.size(); ++i){
        for(int k=offset; k<this->bulkSize; ++k){
            targetStmt->setBindNullAtPos(i+1, k+1);
            nullifyStringAtPos(i, k);
        }
    }
}

void BulkOperationHelper::nullifyStringAtPos(int bufferIx, int dataIx)
{
    copyStringAtPos(bufferIx, dataIx, L"\0");
}

void BulkOperationHelper::copyStringAtPos(int bufferIx, int dataIx, const dtext *str)
{
    const QString &dataType=dataTypes.at(bufferIx);

    if(DbUtil::isStringType(dataType)){
        copyStringToArray((dtext*)buffers.at(bufferIx), dataIx, lenghts.at(bufferIx), str);
    }

    /*
    const QString &dataType=dataTypes.at(bufferIx);

    if(DbUtil::isStringType(dataType)){

        dtext *strBuffer = (dtext*)buffers.at(bufferIx);
        dtext *currPosInBuffer = &strBuffer[dataIx*(lenghts.at(bufferIx)+1)];
        dtscpy(currPosInBuffer, str);

    }*/
}

void BulkOperationHelper::cleanup()
{
    for(int i=0; i<buffers.size(); ++i){
        const QString &dataType=dataTypes.at(i);

        if(DbUtil::isStringType(dataType) || DbUtil::isRawType(dataType)){
            delete[] (dtext*)buffers.at(i);
        }else if(DbUtil::isNumericType(dataType)){
            delete[] (double*)buffers.at(i);
        }else if(DbUtil::isDateType(dataType)){
            OCI_DateArrayFree((OCI_Date**)buffers.at(i));
        }else if(DbUtil::isTimestampType(dataType)){
            OCI_TimestampArrayFree((OCI_Timestamp**)buffers.at(i));
        }else if(DbUtil::isIntervalType(dataType)){
            OCI_IntervalArrayFree((OCI_Interval**)buffers.at(i));
        }else{
            qDebug() << "Unsupported column data type in ~DataComparerThread:" << dataType;
            Q_ASSERT(false);
        }
    }
}
