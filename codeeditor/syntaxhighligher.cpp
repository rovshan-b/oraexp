#include "syntaxhighligher.h"
#include <QFile>
#include <QTextStream>
#include <QSet>
#include "util/strutil.h"
#include "code_parser/stringreader.h"
#include "code_parser/plsql/plsqltokens.h"

SyntaxHighligher::SyntaxHighligher(QTextDocument * parent) :
    QSyntaxHighlighter(parent)
{
    keywordFormat.setForeground(Qt::blue);
    numberFormat.setForeground(Qt::darkGreen);
    singleQuotationFormat.setForeground(QColor(104,34,139));
    doubleQuotationFormat.setForeground(Qt::darkRed);
    singleLineCommentFormat.setForeground(Qt::gray);
    multiLineCommentFormat.setForeground(Qt::gray);
}

void SyntaxHighligher::highlightBlock(const QString &text)
{
    StringReader reader(text);
    scanner.setTextReader(&reader, false);

    int prevState = previousBlockState();
    if(prevState == EndsWithComment){
        scanner.setInitialState(PlSqlScanner::IN_ML_COMMENT);
    }else if(prevState == EndsWithString){
        scanner.setInitialState(PlSqlScanner::IN_QUOTED_STRING);
    }

    QTextCharFormat currentFormat;
    int token = 0;
    PlSqlScanner::ScannerState endState = PlSqlScanner::DONE;
    do{
        token = scanner.getNextToken(false);
        if(scanner.getEndState() != PlSqlScanner::DONE){
            endState = scanner.getEndState();
        }

        bool valid = true;

        switch(token){
        case PLS_QUOTED_STRING:
            currentFormat = singleQuotationFormat;
            break;
        case PLS_NUMBER:
            currentFormat = numberFormat;
            break;
        case PLS_DOUBLEQUOTED_STRING:
            currentFormat = doubleQuotationFormat;
            break;
        case PLS_SL_COMMENT:
            currentFormat = singleLineCommentFormat;
            break;
        case PLS_ML_COMMENT:
            currentFormat = multiLineCommentFormat;
            break;
        default:
            if(token < NON_LITERAL_START_IX){
                currentFormat = keywordFormat;
            }else{
                valid = false;
            }
            break;
        }

        if(valid){
            setFormat(scanner.getTokenStartPos(),
                      scanner.getTokenEndPos() - scanner.getTokenStartPos(),
                      currentFormat);
        }

    }while(token != PLS_E_O_F);

    if(endState == PlSqlScanner::IN_ML_COMMENT){
        setCurrentBlockState(EndsWithComment);
    }else if(endState == PlSqlScanner::IN_QUOTED_STRING){
        setCurrentBlockState(EndsWithString);
    }else{
        setCurrentBlockState(NoState);
    }
}
