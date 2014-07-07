#ifndef PLSQLFORMATTER_H
#define PLSQLFORMATTER_H

#include <QString>
#include <QStack>

class CodeScanner;

class PlSqlFormatter
{
public:   
    enum SelectStatementSection
    {
        SelectList,
        IntoList,
        TableList,
        Where,
        GroupBy,
        Having,
        OrderBy
    };

    static QString format(const QString &code);

    static bool checkPrevToken(const QList<int> &prevTokenList, int token);
    static bool isPrevKeyword(const QList<int> &prevTokenList);
    static int getPrevToken(const QList<int> &prevTokenList);


    static void indent(QString &str, const QStack<QString> &indents);
    static bool indentToEnd(QString &str, QStack<QString> &indents);
    static void increaseIndenting(QStack<QString> &indents);
    static void unindent(QStack<QString> &indents);
    static QString strTab;

    static bool formatGenericConstruct(QString &result, int token, CodeScanner *scanner, QList<int> &prevTokenList, QStack<QString> &indents, bool nested);
    static void formatSelectStatement(QString &result, CodeScanner *scanner, bool nested, QStack<QString> &indents);
    static bool formatParameterList(QString &result, CodeScanner *scanner, QList<int> &prevTokenList, QStack<QString> &indents, int nestingLevel = 0);
    static void formatDefaultToken(QString &result, int token, int prevToken, CodeScanner *scanner, QStack<QString> &indents);

private:
    PlSqlFormatter();
};

#endif // PLSQLFORMATTER_H
