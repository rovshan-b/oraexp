#ifndef OBJECTDDLOPTIONS_H
#define OBJECTDDLOPTIONS_H

#include <QString>
#include "util/param.h"

class ObjectDdlOptions
{
public:
    ObjectDdlOptions();

    QString prettyPrint;
    QString sqlTerminators;
    QString segmentAttributes;
    QString storage;
    QString tablespace;
    QString constraints;
    QString constraintsAsAlter;
    QString refConstraints;
    QString oid;
    QString sizeByteKeyword;

    QList<Param*> getQueryParams() const;

};

#endif // OBJECTDDLOPTIONS_H
