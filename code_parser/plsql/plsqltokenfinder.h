#ifndef PLSQLTOKENFINDER_H
#define PLSQLTOKENFINDER_H

#include <QString>

class PlSqlTreeBuilder;
class ParseTreeNode;

class PlSqlTokenFinder
{
public:
    static void findObjectName(PlSqlTreeBuilder *treeBuilder, QString *schema, QString *name);

private:
    PlSqlTokenFinder();
};

#endif // PLSQLTOKENFINDER_H
