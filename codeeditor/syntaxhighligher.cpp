#include "syntaxhighligher.h"
#include <QFile>
#include <QTextStream>
#include <QSet>
#include "util/strutil.h"
#include "code_parser/plsql/plsqlparsingtable.h"

QStringList SyntaxHighligher::keywords;

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
    //rule.pattern = QRegExp("'.*'");
    //rule.pattern.setMinimal(true);
    //rule.format = singleQuotationFormat;
    //highlightingRules.append(rule);

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

    singleQuotationExpression = QRegExp("'");

    if(SyntaxHighligher::keywords.isEmpty()){
        SyntaxHighligher::keywords.reserve(100);
        QFile f(":/misc/keywords");
        f.open(QIODevice::ReadOnly);
        QTextStream in(&f);
        QString line;
        while(!in.atEnd()){
            line=in.readLine().trimmed();
            if(!line.isEmpty()){
                SyntaxHighligher::keywords.append(line);
            }
        }

        qSort(SyntaxHighligher::keywords.begin(), SyntaxHighligher::keywords.end(), caseInsensitiveLessThan);
    }
}

void SyntaxHighligher::highlightBlock(const QString &text)
{
    //highlight keywords
    QSet<QString> allWords = text.split(QRegExp("\\W+")).toSet();
    foreach(const QString &word, allWords){
        if(!word.isEmpty() && isKeyword(word)){
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

    highlightMultilineConstruct(text, commentStartExpression, commentEndExpression, multiLineCommentFormat, 1);
    highlightMultilineConstruct(text, singleQuotationExpression, singleQuotationExpression, singleQuotationFormat, 2);
}

bool SyntaxHighligher::isKeyword(const QString &word) const
{
    QStringList::const_iterator it=qBinaryFind(SyntaxHighligher::keywords.begin(),
                                               SyntaxHighligher::keywords.end(),
                                               word,
                                               caseInsensitiveLessThan);
    if(it==keywords.end()){
        return false;
    }else{
        return true;
    }
}

void SyntaxHighligher::highlightMultilineConstruct(const QString &text,
                                                   const QRegExp &startExpression,
                                                   const QRegExp &endExpression,
                                                   const QTextCharFormat &format,
                                                   int blockState)
{
    if(currentBlockState()>0){
        return;
    }

    bool onSameLine = false;
    int startIndex = 0;
    if (previousBlockState() != blockState){
        startIndex = startExpression.indexIn(text);
        onSameLine = true;
    }

    while (startIndex >= 0) {
        int endIndex = endExpression.indexIn(text, startIndex + (onSameLine ? 1 : 0));
        int commentLength;
        if (endIndex == -1) {
            setCurrentBlockState(blockState);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                    + endExpression.matchedLength();
        }
        setFormat(startIndex, commentLength, format);
        startIndex = startExpression.indexIn(text, startIndex + commentLength);
    }
}
