#ifndef NEWDBOBJECTSTORAGEOPTIONS_H
#define NEWDBOBJECTSTORAGEOPTIONS_H

#include <QString>

class NewDbObjectStorageOptions
{
public:
    NewDbObjectStorageOptions();

    bool storage;
    bool tablespaceOnly;
    QString tablespaceName;
    QString iotOverflowTablespaceName;
    QString indexTablespaceName;
    QString lobTablespaceName;
    QString partitionsTablespaceName;
};

#endif // NEWDBOBJECTSTORAGEOPTIONS_H
