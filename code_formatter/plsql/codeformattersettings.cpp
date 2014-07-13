#include "codeformattersettings.h"
#include "codeformatterrule.h"
#include "codeformatteraction.h"
#include "code_parser/plsql/plsqlparsingtable.h"
#include "util/filesystemutil.h"
#include <QDomDocument>

CodeFormatterSettings::CodeFormatterSettings()
{
    parsingTable = PlSqlParsingTable::getInstance();

    readSettings();
}

CodeFormatterSettings::~CodeFormatterSettings()
{
    qDeleteAll(rules);
}

QList<CodeFormatterRule*> CodeFormatterSettings::rulesForToken(int token, const QString &lexeme) const
{
    QList<CodeFormatterRule*> result;

    foreach(CodeFormatterRule *rule, rules){
        if(rule->containsToken(token, lexeme)){
            result.append(rule);
        }
    }

    return result;
}

QList<CodeFormatterAction *> CodeFormatterSettings::actionsForToken(int token, const QString &lexeme) const
{
    QList<CodeFormatterAction*> result;

    QList<CodeFormatterRule*> ruleList = rulesForToken(token, lexeme);
    foreach(CodeFormatterRule *rule, ruleList){
        result.append(rule->getActions());
    }

    return result;
}

/*
example:
<?xml version="1.0" encoding="UTF-8"?>
<settings>
    <rule lexemes="DECLARE,BEGIN,INTO,THEN,ELSE,DEFINE" tokens="PLS_SEMI,PLS_SL_COMMENT,PLS_ML_COMMENT">
          <after>
             <action type="newline" />
          </after>
    </rule>
</settings>
*/

void CodeFormatterSettings::readSettings()
{
    QDomDocument doc;
    QString xml = FileSystemUtil::readAsString(":/misc/code_formatter_settings.xml");
    if(!doc.setContent(xml)){
        return;
    }

    QDomElement rootElem = doc.documentElement();
    for(QDomNode n = rootElem.firstChild(); !n.isNull(); n = n.nextSibling())
    {
        processRuleNode(n);
    }
}

void CodeFormatterSettings::processRuleNode(const QDomNode &ruleNode)
{
    CodeFormatterRule *rule = new CodeFormatterRule();

    QDomNamedNodeMap attributes = ruleNode.attributes();
    QDomNode lexemesNode = attributes.namedItem("lexemes");
    if(!lexemesNode.isNull()){
        QString lexemes = lexemesNode.toAttr().value();
        rule->setLexemes( lexemes.split(QRegExp("\\s*,\\s*"), QString::SkipEmptyParts) );
    }

    QDomNode tokensNode = attributes.namedItem("tokens");
    if(!tokensNode.isNull()){
        QString tokensAttrValue = tokensNode.toAttr().value();
        QStringList tokenNames = tokensAttrValue.split(QRegExp("\\s*,\\s*"), QString::SkipEmptyParts);
        foreach(const QString &tokenName, tokenNames){
            rule->addToken(parsingTable->getTokenByName(tokenName));
        }
    }

    rules.append(rule);

    for(QDomNode n = ruleNode.firstChild(); !n.isNull(); n = n.nextSibling())
    {
        processActionNode(n, rule);
    }

    rule->ready();
}

void CodeFormatterSettings::processActionNode(const QDomNode &actionNode, CodeFormatterRule *rule)
{
    QDomElement actionElement = actionNode.toElement();

    QMultiHash<QString,QString> properties;

    QDomNamedNodeMap attributes = actionElement.attributes();
    for(int i=0; i<attributes.size(); ++i){
        QDomAttr attribute = attributes.item(i).toAttr();

        properties.insert(attribute.name(), attribute.value());
    }

    processActionNodeChildren(actionNode, properties);

    CodeFormatterAction *action = new CodeFormatterAction(properties);
    rule->addAction(action);
}

void CodeFormatterSettings::processActionNodeChildren(const QDomNode &actionNode, QMultiHash<QString, QString> &properties)
{
    for(QDomNode n = actionNode.firstChild(); !n.isNull(); n = n.nextSibling())
    {
        QDomElement childElement = n.toElement();
        properties.insert(childElement.tagName(), childElement.attribute("value"));
    }
}
