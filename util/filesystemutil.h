#ifndef FILESYSTEMUTIL_H
#define FILESYSTEMUTIL_H

#include <QString>
#include <QFile>
#include <QTextStream>

class QTextStream;
class QFile;
class DbConnection;

class FileSystemUtil
{
public:
    FileSystemUtil();

    static bool copyFile(const QString &from, const QString &to,
                         QString *errorMessage=0);

    static bool createTextStream(const QString &filename,
                                 const QString &encoding,
                                 bool bom,
                                 QIODevice::OpenMode openMode,
                                 QTextStream **textStream,
                                 QFile **file,
                                 QString *errorMessage);

    static void setTextStreamProperties(QTextStream *textStream, const QString &encoding, bool bom);

    static bool tryRead(const QString &filename, QString *errorMessage);

    static QString readAsString(const QString &filename, QString *errorMessage = 0);

    static QString readTextResource(const QString &prefix, const QString &name, const QString &suffix,
                                    DbConnection *db);

private:
    static QString getResourceFromFile(const QString &prefix, const QString &name, const QString &suffix, DbConnection *db, QStringList &replacedIncludes);
    static QString getFileNameForResource(const QString &prefix, const QString &name, const QString &suffix, DbConnection *db);
    static void replacePlaceholders(const QString &prefix, const QString &suffix, QString &queryText, DbConnection *db, QStringList &replacedIncludes);
};

#endif // FILESYSTEMUTIL_H
