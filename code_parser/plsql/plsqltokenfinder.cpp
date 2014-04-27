#include "plsqltokenfinder.h"
#include "plsqltreebuilder.h"
#include "plsqlrules.h"
#include "beans/parsetreenode.h"
#include "beans/tokeninfo.h"

PlSqlTokenFinder::PlSqlTokenFinder()
{
}

void PlSqlTokenFinder::findObjectName(PlSqlTreeBuilder *treeBuilder, QString *schema, QString *name)
{
    *schema = QString();
    *name = QString();

    ParseTreeNode *objectNameNode = treeBuilder->findNode(treeBuilder->getRootNode(), R_OBJECT_NAME, true);

    if(objectNameNode){
        if(objectNameNode->children.size()==1){
            *name = objectNameNode->children.at(0)->children.at(0)->tokenInfo->lexeme;
        }else{
            Q_ASSERT(objectNameNode->children.size()==3);
            *schema = objectNameNode->children.at(0)->children.at(0)->tokenInfo->lexeme;
            *name = objectNameNode->children.at(2)->children.at(0)->tokenInfo->lexeme;
        }
    }
}
