#ifndef NEWDBOBJECTDDLOPTIONS_H
#define NEWDBOBJECTDDLOPTIONS_H

#include <QString>

class NewDbObjectDdlOptions
{
public:
    NewDbObjectDdlOptions();

    bool storage;
    bool tablespaceOnly;
    QString tablespaceName;
    QString iotOverflowTablespaceName;
    QString indexTablespaceName;
    QString lobTablespaceName;
    QString partitionsTablespaceName;
};

#endif // NEWDBOBJECTDDLOPTIONS_H
