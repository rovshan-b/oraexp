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

    QList<int> objectNamePath = QList<int>() << R_START_RULE << R_CREATE_OBJECT << R_OBJ_TYPE_AND_NAME << R_OBJECT_NAME;
    ParseTreeNode *objectNameNode = treeBuilder->getNode(objectNamePath);

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
