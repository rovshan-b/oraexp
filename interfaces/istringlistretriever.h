#ifndef ISTRINGLISTRETRIEVER_H
#define ISTRINGLISTRETRIEVER_H

#include <QStringList>

class IStringListRetriever
{
public:
    virtual ~IStringListRetriever() {}

    virtual QStringList getStringList(const QString &type=QString()) = 0;
};

#endif // ISTRINGLISTRETRIEVER_H
