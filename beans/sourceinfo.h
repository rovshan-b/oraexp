#ifndef SOURCEINFO_H
#define SOURCEINFO_H

#include "dbobjectinfo.h"

#include <QString>

class SourceInfo : public DbObjectInfo
{
public:
    SourceInfo();
    virtual ~SourceInfo();

    QString owner;
    QString objectName;
    QString objectType;
    QString source;
};

#endif // SOURCEINFO_H
