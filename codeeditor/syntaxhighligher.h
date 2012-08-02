#ifndef SYNTAXHIGHLIGHER_H
#define SYNTAXHIGHLIGHER_H

#include <QSyntaxHighlighter>

#include <QHash>
#include <QTextCharFormat>

class SyntaxHighligher : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    explicit SyntaxHighligher(QTextDocument * parent);

protected:
    void highlightBlock(const QString &text);

private:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };

    QVector <HighlightingRule> highlightingRules;

    QRegExp commentStartExpression;
    QRegExp commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat singleQuotationFormat;
    QTextCharFormat doubleQuotationFormat;
    QTextCharFormat numberFormat;

};

#endif // SYNTAXHIGHLIGHER_H
