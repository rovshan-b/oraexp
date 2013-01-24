#ifndef EXPLAINPLANROW_H
#define EXPLAINPLANROW_H

#include "connectivity/fetchresult.h"

class ExplainPlanRow
{
public:
    ExplainPlanRow();

    int level;
    QString operation;
    QString objectName;
    QString optimizer;
    QString options;
    QString cost;
    QString bytes;
    QString cardinality;
    QString other;
    QString partitionId;
    QString filterPredicates;
    QString accessPredicates;

    static ExplainPlanRow* createFromFetchResult(const FetchResult &result);
};

#endif // EXPLAINPLANROW_H
