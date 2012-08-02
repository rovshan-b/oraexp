#ifndef TABLEINFOFORDATACOMPARISON_H
#define TABLEINFOFORDATACOMPARISON_H

#include <QString>
#include <QStringList>

class TableInfoForDataComparison
{
public:
    TableInfoForDataComparison();

    QString whereClause;
    QString targetTableName;
    //QStringList columns;
    QStringList columnsToCompare;

    void addColumnToCompare(const QString &name);
};

#endif // TABLEINFOFORDATACOMPARISON_H
