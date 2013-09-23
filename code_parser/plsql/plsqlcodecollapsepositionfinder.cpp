#include "plsqlcodecollapsepositionfinder.h"
#include "plsqlscanner.h"
#include "plsqltokens.h"
#include "../textcursorreader.h"
#include "beans/codecollapseposition.h"
#include <QStack>

PlSqlCodeCollapsePositionFinder::PlSqlCodeCollapsePositionFinder()
{
}

QList<CodeCollapsePosition *> PlSqlCodeCollapsePositionFinder::findCodeCollapsePositions(const QTextCursor &cur)
{
    QList<CodeCollapsePosition*> positions;
    QScopedPointer<PlSqlScanner> scanner(new PlSqlScanner(new TextCursorReader(cur)));

    QStack<CodeCollapsePosition*> collapsePositionsStack;

    QRegExp blockStartLexeme("BEGIN|IF|LOOP|CASE", Qt::CaseInsensitive);
    QString blockEndLexeme = "END";
    QString lastLexeme, prevLexeme;

    int token;

    do{
        token = scanner->getNextToken();
        prevLexeme = lastLexeme;
        lastLexeme = scanner->getTokenLexeme();

        if(blockStartLexeme.exactMatch(lastLexeme) && prevLexeme.compare(blockEndLexeme, Qt::CaseInsensitive)!=0){
            CodeCollapsePosition* collapsePos = new CodeCollapsePosition();
            collapsePos->startLine = scanner->getTokenStartLine();

            collapsePositionsStack.push(collapsePos);
        }else if(lastLexeme.compare(blockEndLexeme, Qt::CaseInsensitive)==0){
            if(!collapsePositionsStack.isEmpty()){
                CodeCollapsePosition *collapsePos = collapsePositionsStack.pop();
                collapsePos->endLine = scanner->getTokenEndLine();

                positions.append(collapsePos);
            }
        }
    }while(token!=PLS_E_O_F);

    //close all open blocks
    while(!collapsePositionsStack.isEmpty()){
        CodeCollapsePosition *collapsePos = collapsePositionsStack.pop();
        collapsePos->endLine = scanner->getTokenEndLine();

        positions.append(collapsePos);
    }

    //clear positions with same start lines. inner blocks are added first, so need to loop from top to bottom
    bool changed;

    do{
        changed = false;
        for(int i=0; i<positions.size(); ++i){
            bool hasSameStartPos = containsSameStartPos(positions, positions.at(i)->startLine, i+1);

            if(hasSameStartPos){
                changed = true;
                positions.removeFirst();
            }
        }
    }while(changed);

    return positions;
}

bool PlSqlCodeCollapsePositionFinder::containsSameStartPos(const QList<CodeCollapsePosition *> &positions, int startLine, int startIx)
{
    CodeCollapsePosition *collapsePos;

    for(int i=startIx; i<positions.size(); ++i){
        collapsePos = positions.at(i);
        if(collapsePos->startLine == startLine){
            return true;
        }else if(i>startIx){ //no need to scan further if block right next to startIx does not have same start line
            return false;
        }
    }

    return false;
}
