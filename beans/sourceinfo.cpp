#include "sourceinfo.h"

SourceInfo::SourceInfo()
{
}

SourceInfo::~SourceInfo()
{
    qDebug("deleted SourceInfo");
}

bool SourceInfo::needsRecreation(DbObjectInfo *other) const
{
    SourceInfo *info = static_cast<SourceInfo*>(other);
    return source!=info->source;
}
