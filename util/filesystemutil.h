#ifndef FILESYSTEMUTIL_H
#define FILESYSTEMUTIL_H

#include <QString>
#include <QFile>
#include <QTextStream>

class QTextStream;
class QFile;

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
};

#endif // FILESYSTEMUTIL_H
