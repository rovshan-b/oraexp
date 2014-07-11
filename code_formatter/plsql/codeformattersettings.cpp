#include "codeformattersettings.h"
#include "codeformatterrule.h"
#include "codeformatteraction.h"
#include "util/filesystemutil.h"
#include <QDomDocument>

CodeFormatterSettings::CodeFormatterSettings()
{
    readSettings();
}

CodeFormatterSettings::~CodeFormatterSettings()
{
    qDeleteAll(rules);
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
    rules.append(rule);

    for(QDomNode n = ruleNode.firstChild(); !n.isNull(); n = n.nextSibling())
    {
        processActionNode(n, rule);
    }
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

    CodeFormatterAction *action = new CodeFormatterAction(properties);
    rule->addAction(action);
}
