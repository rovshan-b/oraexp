#include "objectddloptions.h"

ObjectDdlOptions::ObjectDdlOptions() : prettyPrint("y")
{
}

QList<Param*> ObjectDdlOptions::getQueryParams() const
{
    QList<Param*> params;

    params.append(new Param(":p_pretty", prettyPrint));
    params.append(new Param(":p_sqlterm", sqlTerminators));
    params.append(new Param(":p_segattr", segmentAttributes));
    params.append(new Param(":p_storage", storage));
    params.append(new Param(":p_tablespace", tablespace));
    params.append(new Param(":p_cons", constraints));
    params.append(new Param(":p_cons_as_alter", constraintsAsAlter));
    params.append(new Param(":p_ref_cons", refConstraints));
    params.append(new Param(":p_oid", oid));
    params.append(new Param(":p_size_byte_keyword", sizeByteKeyword));

    return params;
}
