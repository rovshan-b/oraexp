#ifndef PLSQLFORMATTER_H
#define PLSQLFORMATTER_H

#include <QString>
#include <QStack>
#include "codeformattersettings.h"

class PlSqlScanner;

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

    PlSqlFormatter();

    QString format(const QString &code);

private:
    CodeFormatterSettings settings;

    QString result;

    PlSqlScanner *scanner;

    QList<int> prevTokenList;
    QStack<QString> indents;

    bool checkPrevToken(int token);
    bool isPrevKeyword();
    int getPrevToken();


    void indent(QString &str);
    bool indentToEnd(QString &str);
    void increaseIndenting();
    void unindent();
    static QString strTab;

    bool formatGenericConstruct(int token, bool nested);
    void formatSelectStatement(bool nested);
    bool formatParameterList(int nestingLevel = 0);
    void formatDefaultToken(int token);
};

#endif // PLSQLFORMATTER_H
