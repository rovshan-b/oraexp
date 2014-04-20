#ifndef SYNTAXHIGHLIGHER_H
#define SYNTAXHIGHLIGHER_H

#include <QSyntaxHighlighter>
#include <QHash>
#include <QTextCharFormat>
#include "code_parser/plsql/plsqlscanner.h"

class BlockData;

class SyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    explicit SyntaxHighlighter(QTextDocument * parent);

protected:
    void highlightBlock(const QString &text);

private:
    enum BlockState
    {
        NoState,
        EndsWithComment,
        EndsWithString
    };

    PlSqlScanner scanner;

    QTextCharFormat keywordFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat singleQuotationFormat;
    QTextCharFormat doubleQuotationFormat;
    QTextCharFormat numberFormat;

    BlockData *getBlockData();
};

#endif // SYNTAXHIGHLIGHER_H
