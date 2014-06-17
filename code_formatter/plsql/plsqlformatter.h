#ifndef PLSQLFORMATTER_H
#define PLSQLFORMATTER_H

#include <QString>

class PlSqlFormatter
{
public:   
    static QString format(const QString &code);

    static bool checkPrevToken(const QList<int> &prevTokenList, int token);
    static bool isPrevKeyword(const QList<int> &prevTokenList);

private:
    PlSqlFormatter();
};

#endif // PLSQLFORMATTER_H
