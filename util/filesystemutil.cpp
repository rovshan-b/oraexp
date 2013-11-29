#include "filesystemutil.h"
#include <QStringList>
#include "connectivity/dbconnection.h"
#include <iostream>
#include <stdexcept>
#include <QDebug>

FileSystemUtil::FileSystemUtil()
{
}

bool FileSystemUtil::copyFile(const QString &from, const QString &to, QString *errorMessage)
{
    QFile sourceFile(from);
    QFile destFile(to);

    bool success = sourceFile.open(QIODevice::ReadOnly);
    if(!success){
        *errorMessage = sourceFile.errorString();
        return false;
    }
    success = destFile.open(QIODevice::WriteOnly);
    if(!success){
        sourceFile.close();
        *errorMessage = destFile.errorString();
        return false;
    }

    destFile.write(sourceFile.readAll());

    sourceFile.close();
    destFile.close();

    return true;
}

bool FileSystemUtil::createTextStream(const QString &filename,
                                      const QString &encoding,
                                      bool bom,
                                      QIODevice::OpenMode openMode,
                                      QTextStream **textStream,
                                      QFile **file,
                                      QString *errorMessage)
{
    *file = new QFile(filename);
    if(!(*file)->open(openMode)){
        *errorMessage = (*file)->errorString();
        delete *file;
        *file = 0;
        return false;
    };

    *textStream = new QTextStream(*file);
    FileSystemUtil::setTextStreamProperties(*textStream, encoding, bom);

    return true;
}

void FileSystemUtil::setTextStreamProperties(QTextStream *textStream, const QString &encoding, bool bom)
{
    textStream->setCodec(encoding.toStdString().c_str());
    textStream->setGenerateByteOrderMark(bom);
}

bool FileSystemUtil::tryRead(const QString &filename, QString *errorMessage)
{
    QFile file(filename);
    if(!file.exists()){
        *errorMessage = QObject::tr("File does not exist");
        return false;
    }

    if(!file.open(QIODevice::ReadOnly)){
        *errorMessage = file.errorString();
        return false;
    }

    return true;
}

QString FileSystemUtil::readAsString(const QString &filename, QString *errorMessage)
{
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        if(errorMessage){
            *errorMessage = file.errorString();
        }
        return "";
    }

    QTextStream in(&file);
    in.setCodec("UTF-8");
    QString result = in.readAll();
    file.close();

    return result;
}

QString FileSystemUtil::readTextResource(const QString &prefix, const QString &name, const QString &suffix, DbConnection *db)
{
    QStringList replacedIncludes;
    return FileSystemUtil::getResourceFromFile(prefix, name, suffix, db, replacedIncludes);
}

QString FileSystemUtil::getResourceFromFile(const QString &prefix, const QString &name,
                                         const QString &suffix, DbConnection *db,
                                         QStringList &replacedIncludes)
{
    if(name.isEmpty()){
        return "";
    }

    QString filenameForQuery=getFileNameForResource(prefix, name, suffix, db);
    QFile queriesFile(filenameForQuery);
    if(!queriesFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "resource file not found. file name =" << filenameForQuery;
        throw std::runtime_error("query file not found ");
    }
    QByteArray fileData=queriesFile.readAll();
    queriesFile.close();
    QString fileText = QString(fileData);

    FileSystemUtil::replacePlaceholders(prefix, suffix, fileText, db, replacedIncludes);

    return fileText;
}

QString FileSystemUtil::getFileNameForResource(const QString &prefix, const QString &name, const QString &suffix,
                                               DbConnection *db)
{
    Q_ASSERT(!name.isEmpty());

    if(db==0){
        return QString("%1%2.%3").arg(prefix, name, suffix);
    }

    //try to find closest version number
    QString versionedFileName;
    int serverMajorVersion=db->getServerMajorVersion();
    const int minimumSupportedVersion=9;
    while(serverMajorVersion > minimumSupportedVersion){
        versionedFileName=QString("%1%2_%3.%4").arg(prefix, name).arg(serverMajorVersion).arg(suffix);
        if(QFile::exists(versionedFileName)){
            return versionedFileName;
        }

        serverMajorVersion--;
    }

    return QString("%1%2.%3").arg(prefix, name, suffix);
}

void FileSystemUtil::replacePlaceholders(const QString &prefix,
                                         const QString &suffix,
                                         QString &queryText,
                                         DbConnection *db,
                                         QStringList &replacedIncludes)
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

        queryText.replace(regexpInc.pos(), regexpInc.matchedLength(), FileSystemUtil::getResourceFromFile(prefix, capture, suffix, db, replacedIncludes));
        replacedIncludes.append(capture);
    }
}
