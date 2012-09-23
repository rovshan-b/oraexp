#include "dblinkinfo.h"

DbLinkInfo::DbLinkInfo() : shared(false)
{
}

DbLinkInfo::~DbLinkInfo()
{
    qDebug("DbLinkInfo deleted");
}

DbLinkInfo DbLinkInfo::fromFetchResult(const FetchResult &result)
{
    DbLinkInfo i;

    i.owner=result.colValue("OWNER");
    i.name=result.colValue("DB_LINK");
    i.username=result.colValue("USERNAME");
    i.host=result.colValue("HOST");

    return i;
}
