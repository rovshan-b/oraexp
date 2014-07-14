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

    enum UpdateStatementSection
    {
        UpdateTableList,
        UpdateSetClause,
        UpdateRestOfStatement
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
    void chopLastIndent();
    static QString strTab;

    void addLineBreak();

    bool formatGenericConstruct(bool nested);
    void formatSelectStatement(bool nested);
    void formatUpdateStatement();
    bool formatParameterList(int closingToken, int nestingLevel = 0);
    void formatDefaultToken();

    void applyAction(CodeFormatterAction *action,
                     TokenInfo *prevTokenInfo, const QList<CodeFormatterAction *> &prevTokenActions);

    bool containsAction(const QList<CodeFormatterAction *> &actions,
                        CodeFormatterAction::ActionSequence sequence,
                        CodeFormatterAction::ActionType type) const;

    void readNextToken();
    bool isBracket() const;
    int getClosingBracket() const;
};

#endif // PLSQLFORMATTER_H
