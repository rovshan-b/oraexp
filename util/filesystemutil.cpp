#include "filesystemutil.h"
#include <QFile>

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
