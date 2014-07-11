#ifndef CODEFORMATTERSETTINGS_H
#define CODEFORMATTERSETTINGS_H

#include <QList>
#include <QDomNode>

class CodeFormatterRule;

class CodeFormatterSettings
{
public:
    CodeFormatterSettings();
    ~CodeFormatterSettings();

private:
    void readSettings();
    void processRuleNode(const QDomNode &ruleNode);
    void processActionNode(const QDomNode &actionNode, CodeFormatterRule *rule);

    QList<CodeFormatterRule*> rules;
};

#endif // CODEFORMATTERSETTINGS_H
