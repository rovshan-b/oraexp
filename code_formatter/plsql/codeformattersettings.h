#ifndef CODEFORMATTERSETTINGS_H
#define CODEFORMATTERSETTINGS_H

#include <QList>
#include <QDomNode>
#include "codeformatteraction.h"

class CodeFormatterRule;
class PlSqlParsingTable;

class CodeFormatterSettings
{
public:
    CodeFormatterSettings();
    ~CodeFormatterSettings();

    QList<CodeFormatterRule*> rulesForToken(int token, const QString &lexeme) const;
    QList<CodeFormatterAction*> actionsForToken(int token, const QString &lexeme) const;

private:
    void readSettings();
    void processRuleNode(const QDomNode &ruleNode);
    void processActionNode(const QDomNode &actionNode, CodeFormatterRule *rule);
    void processActionNodeChildren(const QDomNode &actionNode, QMultiHash<QString,QString> &properties);

    QList<CodeFormatterRule*> rules;

    PlSqlParsingTable *parsingTable;
};

#endif // CODEFORMATTERSETTINGS_H
