#ifndef PLSQLFORMATTER_H
#define PLSQLFORMATTER_H

#include <QString>
#include <QStack>
#include "codeformattersettings.h"
#include "scopeinfo.h"

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
    QList<ScopeInfo> scope;

    bool checkPrevToken(int token);
    bool isPrevKeyword();
    TokenInfo *getPrevToken(bool ignoreWhitespace = true);


    void indent(QString &str);
    void indentToEnd(QString &str, bool additionalSpace = false);
    void increaseIndenting();
    void unindent();
    void chopLastIndent();
    static QString strTab;

    void addLineBreak();

    bool formatGenericConstruct();
    bool formatParameterList(int closingToken, int nestingLevel = 0);
    void formatDefaultToken();

    void executeActions(QList<CodeFormatterAction*> actions, CodeFormatterAction::ActionSequence sequence,
                        TokenInfo *prevTokenInfo, QList<CodeFormatterAction*> prevTokenActions);

    void applyAction(CodeFormatterAction *action,
                     TokenInfo *prevTokenInfo, const QList<CodeFormatterAction *> &prevTokenActions);

    bool containsAction(const QList<CodeFormatterAction *> &actions,
                        CodeFormatterAction::ActionSequence sequence,
                        CodeFormatterAction::ActionType type) const;

    void readNextToken();
    bool isBracket() const;
    int getClosingBracket() const;
    bool isWhitespace() const;

    ScopeInfo currentScope() const;
    QString currentScopeName() const;
    void enterScope(const QString &scopeName, int maxParamCount);
    void exitScope(const QString &scopeName);

    int getParamCountPerLine() const;

};

#endif // PLSQLFORMATTER_H
