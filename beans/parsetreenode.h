#ifndef PARSETREENODE_H
#define PARSETREENODE_H

#include <QList>
#include "tokeninfo.h"

#include <QDebug>

class ParseTreeNodeScope;

class ParseTreeNode
{
public:
    ParseTreeNode();
    virtual ~ParseTreeNode();

    ParseTreeNode *parentNode;
    QList<ParseTreeNode*> children;

    TokenInfo *tokenInfo;
    inline TokenInfo *getTokenInfo() const {return this->tokenInfo;}

    int symbolCount;

    ParseTreeNodeScope *scope;
    bool ownsScope;

    inline bool containsPosition(int position){
        return position>=tokenInfo->startPos && position<tokenInfo->endPos;
    }

    ParseTreeNode *findChildForPosition(int position, bool topLevel = false);

};

inline bool parseTreeNodeLessThan(ParseTreeNode *n1, ParseTreeNode *n2)
{
    return n1->tokenInfo->startPos < n2->tokenInfo->startPos;
}

#endif // PARSETREENODE_H
