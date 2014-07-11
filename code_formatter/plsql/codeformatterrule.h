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

private:
    QStringList lexemes;
    QList<int> tokens;
    QList<CodeFormatterAction*> actions;
};

#endif // CODEFORMATTERRULE_H
