#include "filesystemutil.h"

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
        *errorMessage = file.errorString();
        return "";
    }

    QTextStream in(&file);
    in.setCodec("UTF-8");
    QString result = in.readAll();
    file.close();

    return result;
}
