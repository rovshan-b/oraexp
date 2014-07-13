#ifndef PLSQLFORMATTER_H
#define PLSQLFORMATTER_H

#include <QString>
#include <QStack>
#include "codeformattersettings.h"

class PlSqlScanner;
class TokenInfo;

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
        Model,
        OrderBy,
        RowLimiting,
        ForUpdate
    };

    PlSqlFormatter();
    ~PlSqlFormatter();

    QString format(const QString &code);

private:
    CodeFormatterSettings settings;

    QString result;

    PlSqlScanner *scanner;

    TokenInfo *currTokenInfo;
    int token;
    QList<TokenInfo*> prevTokenList;
    QStack<QString> indents;

    bool checkPrevToken(int token);
    bool isPrevKeyword();
    TokenInfo *getPrevToken();


    void indent(QString &str);
    bool indentToEnd(QString &str, bool additionalSpace = false);
    void increaseIndenting();
    void unindent();
    static QString strTab;

    bool formatGenericConstruct(int token, bool nested);
    void formatSelectStatement(bool nested);
    bool formatParameterList(int nestingLevel = 0);
    void formatDefaultToken();

    void applyAction(CodeFormatterAction *action,
                     const QList<CodeFormatterAction *> &prevTokenActions);

    bool containsAction(const QList<CodeFormatterAction *> &actions,
                        CodeFormatterAction::ActionSequence sequence,
                        CodeFormatterAction::ActionType type) const;

    void readNextToken();
};

#endif // PLSQLFORMATTER_H
