#ifndef SCHEMASELECTORDELEGATE_H
#define SCHEMASELECTORDELEGATE_H

#include "dbitemlistdelegate.h"

class IQueryScheduler;

class SchemaSelectorDelegate : public DbItemListDelegate
{
    Q_OBJECT
public:
    explicit SchemaSelectorDelegate(const QString &schemaName, IQueryScheduler *queryScheduler, QObject *parent, bool appendRowIfLast=false);

    void setDefaultSchemaName(const QString &schemaName){this->initialValue=schemaName;}
};

#endif // SCHEMASELECTORDELEGATE_H
