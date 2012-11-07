#ifndef SCHEMASELECTORDELEGATE_H
#define SCHEMASELECTORDELEGATE_H

#include "dbitemlistdelegate.h"

class IQueryScheduler;

class SchemaSelectorDelegate : public DbItemListDelegate
{
    Q_OBJECT
public:
    explicit SchemaSelectorDelegate(const QString &schemaName, IQueryScheduler *queryScheduler, QObject *parent, bool appendRowIfLast=false);
};

#endif // SCHEMASELECTORDELEGATE_H
