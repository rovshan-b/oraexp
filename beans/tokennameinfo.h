#ifndef TOKENNAMEINFO_H
#define TOKENNAMEINFO_H

#include <QStringList>
#include <QPair>

class TokenNameInfo
{
public:
    TokenNameInfo();

    QStringList parts;
    QList< QPair<int,int> > absolutePositions;

    int currentPartId;

    bool isEmpty() const;

    QString toString() const;
    QString currentLexeme() const;
};

#endif // TOKENNAMEINFO_H
