#ifndef EXPLAINPLANROW_H
#define EXPLAINPLANROW_H

#include <QStringList>

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

    static ExplainPlanRow* createFromStringList(const QStringList &list);
};

#endif // EXPLAINPLANROW_H
