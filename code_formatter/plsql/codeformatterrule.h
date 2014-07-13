#ifndef CODEFORMATTERRULE_H
#define CODEFORMATTERRULE_H

#include <QList>
#include <QStringList>

class CodeFormatterAction;

class CodeFormatterRule
{
public:
    CodeFormatterRule();
    ~CodeFormatterRule();

    void addAction(CodeFormatterAction *action);
    QList<CodeFormatterAction*> getActions() const;

    void setLexemes(const QStringList &lexemes);
    void addToken(int token);

    void ready();

    bool containsToken(int token, const QString &lexeme) const;


private:
    QStringList lexemes;
    QList<int> tokens;
    QList<CodeFormatterAction*> actions;
};

#endif // CODEFORMATTERRULE_H
