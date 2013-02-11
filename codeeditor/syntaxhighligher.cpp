#include "syntaxhighligher.h"
#include <QFile>
#include <QTextStream>
#include <QSet>
#include "util/strutil.h"
#include "code_parser/plsql/plsqlparsingtable.h"

SyntaxHighligher::SyntaxHighligher(QTextDocument * parent) :
    QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(Qt::blue);
    //keywordFormat.setFontWeight(QFont::DemiBold);
    keywordFormat.setFontCapitalization(QFont::AllUppercase);

    numberFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("\\b\\d+\\.?\\d*\\b");
    rule.format=numberFormat;
    highlightingRules.append(rule);

    singleQuotationFormat.setForeground(QColor(104,34,139));
    rule.pattern = QRegExp("'.*'");
    rule.pattern.setMinimal(true);
    rule.format = singleQuotationFormat;
    highlightingRules.append(rule);

    doubleQuotationFormat.setForeground(Qt::darkRed);
    rule.pattern = QRegExp("\".*\"");
    rule.pattern.setMinimal(true);
    rule.format = doubleQuotationFormat;
    highlightingRules.append(rule);

    singleLineCommentFormat.setForeground(Qt::gray);
    rule.pattern = QRegExp("--[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::gray);

    commentStartExpression = QRegExp("/\\*");
    commentEndExpression = QRegExp("\\*/");
}

void SyntaxHighligher::highlightBlock(const QString &text)
{
    //highlight keywords
    QSet<QString> allWords = text.split(QRegExp("\\W+")).toSet();
    foreach(const QString &word, allWords){
        bool isKeyword=(PlSqlParsingTable::getInstance()->getKeywordIx(word)>=0);
        if(isKeyword){
            QRegExp rx(QString("\\b%1\\b").arg(word));
            rx.setCaseSensitivity(Qt::CaseInsensitive);

            int ix=text.indexOf(rx);
            while(ix!=-1){
                setFormat(ix, word.length(), keywordFormat);
                ix=text.indexOf(rx, ix+1);
            }
        }
    }

    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }

    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = commentStartExpression.indexIn(text);

    while (startIndex >= 0) {
        int endIndex = commentEndExpression.indexIn(text, startIndex);
        int commentLength;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                    + commentEndExpression.matchedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
    }
}
