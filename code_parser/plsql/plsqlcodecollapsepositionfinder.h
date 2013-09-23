#ifndef PLSQLCODECOLLAPSEPOSITIONFINDER_H
#define PLSQLCODECOLLAPSEPOSITIONFINDER_H

#include <QList>
#include <QTextCursor>

class PlSqlTreeBuilder;
class CodeCollapsePosition;
class ParseTreeNode;

class PlSqlCodeCollapsePositionFinder
{
public:
    PlSqlCodeCollapsePositionFinder();

    QList<CodeCollapsePosition*> findCodeCollapsePositions(const QTextCursor &cur);

private:
    bool containsSameStartPos(const QList<CodeCollapsePosition*> &positions, int startLine, int startIx);

};

#endif // PLSQLCODECOLLAPSEPOSITIONFINDER_H
