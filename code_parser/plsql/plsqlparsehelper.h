#ifndef PLSQLPARSEHELPER_H
#define PLSQLPARSEHELPER_H

#include <QStringList>

class PlSqlParseHelper
{
public:
    static QStringList getBindParams(const QString &query);

private:
    PlSqlParseHelper();
};

#endif // PLSQLPARSEHELPER_H
