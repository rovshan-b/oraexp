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

    QMultiHash<QString, ParseTreeNode*> declarations;
};

#endif // PARSETREENODESCOPE_H
