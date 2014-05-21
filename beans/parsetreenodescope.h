#ifndef PARSETREENODESCOPE_H
#define PARSETREENODESCOPE_H

#include <QList>
#include <QHash>

class ParseTreeNode;

class ParseTreeNodeScope
{
public:
    ParseTreeNodeScope();
    ~ParseTreeNodeScope();

    QHash<QString, ParseTreeNode*> declarations;
};

#endif // PARSETREENODESCOPE_H
