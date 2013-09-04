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

    static QStringList keywords;
    bool isKeyword(const QString &word) const;

    QVector <HighlightingRule> highlightingRules;

    QRegExp commentStartExpression;
    QRegExp commentEndExpression;

    QRegExp singleQuotationExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat singleQuotationFormat;
    QTextCharFormat doubleQuotationFormat;
    QTextCharFormat numberFormat;

    void highlightMultilineConstruct(const QString &text, const QRegExp &startExpression, const QRegExp &endExpression, const QTextCharFormat &format, int blockState);

};

#endif // SYNTAXHIGHLIGHER_H
