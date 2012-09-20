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

    virtual bool needsRecreation(DbObjectInfo *other) const;
};

#endif // SOURCEINFO_H
