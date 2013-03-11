#ifndef FILESYSTEMUTIL_H
#define FILESYSTEMUTIL_H

#include <QString>

class FileSystemUtil
{
public:
    FileSystemUtil();

    static bool copyFile(const QString &from, const QString &to,
                         QString *errorMessage=0);
};

#endif // FILESYSTEMUTIL_H
