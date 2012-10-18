#include "queryutil.h"
#include "connectivity/dbconnection.h"
#include <QFile>
#include <iostream>
#include <stdexcept>
#include <QDebug>


QHash<QString,QString> QueryUtil::queries;

QueryUtil::QueryUtil()
{
}

QString QueryUtil::getQuery(const QString &queryName, DbConnection *db)
{
    QString cacheKeyName=db==0 ? queryName : QString("%1_%2").arg(queryName).arg(db->getServerMajorVersion());

    if(queries.contains(cacheKeyName)){
        return queries.value(cacheKeyName);
    }else{

        QStringList replacedIncludes; //this helps to prevent multiple inclusion of a same file
        QString fileText=getQueryFromFile(queryName, db, replacedIncludes);

        queries.insert(cacheKeyName, fileText);

        return fileText;
    }
}

QString QueryUtil::getQueryFromFile(const QString &queryName, DbConnection *db, QStringList &replacedIncludes)
{
    if(queryName.isEmpty()){
        return "";
    }

    QString filenameForQuery=getFileNameForQuery(queryName, db);
    QFile queriesFile(filenameForQuery);
    if(!queriesFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "query file not found. file name =" << filenameForQuery;
        throw std::runtime_error("query file not found ");
    }
    QByteArray fileData=queriesFile.readAll();
    queriesFile.close();
    QString fileText = QString(fileData);

    QueryUtil::replacePlaceholders(fileText, db, replacedIncludes);

    return fileText;
}

QString QueryUtil::getFileNameForQuery(const QString &queryName, DbConnection *db)
{
    Q_ASSERT(!queryName.isEmpty());

    if(db==0){
        return QString(":/queries/%1.sql").arg(queryName);
    }

    //try to find closest version number
    QString versionedFileName;
    int serverMajorVersion=db->getServerMajorVersion();
    const int minimumSupportedVersion=9;
    while(serverMajorVersion > minimumSupportedVersion){
        versionedFileName=QString(":/queries/%1_%2.sql").arg(queryName).arg(serverMajorVersion);
        if(QFile::exists(versionedFileName)){
            return versionedFileName;
        }

        serverMajorVersion--;
    }

    return QString(":/queries/%1.sql").arg(queryName);
}

void QueryUtil::replacePlaceholders(QString &queryText, DbConnection *db, QStringList &replacedIncludes)
{
    QRegExp  regexpReplace("\\{@(\\w+):(>=|>|<|<=|==)(\\d{1,2})(?:\\.(\\d{1}))?\\}(.*)\\{\\}", Qt::CaseSensitive, QRegExp::RegExp2);
    regexpReplace.setMinimal(true);
    /*
      example input:
        {@keep_if:==11.2}
        some
        text here
        {}

      matches:
      group(0) = {@keep_if:==11.2} some text here {}
      group(1) = keep_if
      group(2) = ==
      group(3) = 11
      group(4) = 2
      group(5) = some text here
    */

    QString replaceType;
    QString check;
    int majorVersion;
    int minorVersion;
    QString content;
    bool conversionOk;

    while (db!=0 && regexpReplace.indexIn(queryText, 0) != -1) {
        Q_ASSERT(regexpReplace.captureCount()==5);
        replaceType=regexpReplace.cap(1);
        Q_ASSERT(replaceType=="keep_if");
        check=regexpReplace.cap(2);
        majorVersion=regexpReplace.cap(3).toUInt(&conversionOk);
        Q_ASSERT(conversionOk);
        minorVersion=regexpReplace.cap(4).isEmpty() ? -1 : regexpReplace.cap(4).toInt(&conversionOk);
        Q_ASSERT(conversionOk);
        content=regexpReplace.cap(5);

        int versionCheckResult=db->compareVersion(majorVersion, minorVersion);

        if((check==">" && !(versionCheckResult>0)) ||
                (check==">=" && !(versionCheckResult>=0)) ||
                (check=="==" && versionCheckResult!=0) ||
                (check=="<=" && !(versionCheckResult<=0)) ||
                (check=="<" && !(versionCheckResult<0))){

            content="";
        }

        queryText.replace(regexpReplace.pos(), regexpReplace.matchedLength(), content);
    }


    QRegExp regexpInc("\\{@replace_with:(\\w+)\\}", Qt::CaseSensitive, QRegExp::RegExp2);
    QString capture;
    while (regexpInc.indexIn(queryText, 0) != -1) {
        capture=regexpInc.cap(1);

        if(replacedIncludes.contains(capture)){
            //qDebug() << "Infinite recursion detected in QueryUtil::replacePlaceholders while processing query" << queryText;
            //throw std::runtime_error("Infinite recursion detected in QueryUtil::replacePlaceholders");
            capture=""; //replace with empty text, because it has already been included
        }

        queryText.replace(regexpInc.pos(), regexpInc.matchedLength(), QueryUtil::getQueryFromFile(capture, db, replacedIncludes));
        replacedIncludes.append(capture);
    }
}
