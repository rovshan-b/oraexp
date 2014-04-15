#ifndef SYNTAXHIGHLIGHER_H
#define SYNTAXHIGHLIGHER_H

#include <QSyntaxHighlighter>
#include <QHash>
#include <QTextCharFormat>

#include "code_parser/plsql/plsqlscanner.h"

class SyntaxHighligher : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    explicit SyntaxHighligher(QTextDocument * parent);

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

};

#endif // SYNTAXHIGHLIGHER_H
