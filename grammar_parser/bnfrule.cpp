#include "bnfrule.h"
#include "ebnftoken.h"
#include "bnfruleitem.h"
#include <QStringList>

BNFRule::BNFRule() : isStartRule(false), subruleCount(0),
    isReachableFromStartSymbol(false),
    ruleDefId(-1), ruleOptions(0)
{

}

BNFRule::~BNFRule()
{
    for(int i=0; i<alternatives.size(); ++i){
        QList <BNFRuleItem*> items = alternatives.at(i);
        qDeleteAll(items);
    }

    delete ruleOptions;

    //qDeleteAll(firstSet);
}

void BNFRule::startAlternatives()
{
    alternatives.append(QList<BNFRuleItem*>());
}

void BNFRule::addRuleItem(BNFRuleItem *item){
    alternatives[alternatives.size()-1].append(item);
}

void BNFRule::readOptions(const QString &options)
{
    Q_ASSERT(ruleOptions==0);
    Q_ASSERT(!options.isEmpty());

    BNFRuleOption *option = new BNFRuleOption();

    QStringList parts = options.split(',');
    foreach(const QString &part, parts){
        if(part.compare("skip")==0){
            option->skip = true;
        }else if(part.compare("no_children")==0){
            option->noChildren = true;
        }else if(part.compare("scope")==0){
            option->scope = true;
        }else if(part.compare("symbol_table_entry")==0){
            option->symbolTableEntry = true;
        }else if(part.compare("gui_skip")==0){
            option->skipInGuiTree = true;
        }else if(part.startsWith("gui_handler:")){
            option->guiHandlerName = part.split(':').at(1);
        }else if(part.startsWith("gui_icon:")){
            option->guiIconName = part.split(':').at(1);
        }else{
            Q_ASSERT(false);
        }
    }

    Q_ASSERT(! (option->skip && option->noChildren));
    //Q_ASSERT(! (option->scope && option->symbolTableEntry));
    Q_ASSERT(! (option->skip && option->scope));
    Q_ASSERT(! (option->noChildren && option->scope));
    Q_ASSERT(! (option->skipInGuiTree && !option->guiHandlerName.isEmpty()));

    ruleOptions = option;
}

QString BNFRule::codeForOptions() const
{
    Q_ASSERT(ruleOptions!=0 && ruleDefId!=-1);

    QString str;

    QString varName = QString("options_for_%1").arg(ruleName);
    str.append("BNFRuleOption *").append(varName).append(" = new BNFRuleOption();\n");
    str.append(varName).append("->skip = ").append(ruleOptions->skip ? "true" : "false").append(";\n");
    str.append(varName).append("->noChildren = ").append(ruleOptions->noChildren ? "true" : "false").append(";\n");
    str.append(varName).append("->scope = ").append(ruleOptions->scope ? "true" : "false").append(";\n");
    str.append(varName).append("->symbolTableEntry = ").append(ruleOptions->symbolTableEntry ? "true" : "false").append(";\n");
    str.append(varName).append("->skipInGuiTree = ").append(ruleOptions->skipInGuiTree ? "true" : "false").append(";\n");
    if(!ruleOptions->guiHandlerName.isEmpty()){
        str.append(varName).append("->guiHandlerName = \"").append(ruleOptions->guiHandlerName).append("\";\n");
    }
    if(!ruleOptions->guiIconName.isEmpty()){
        str.append(varName).append("->guiIconName = \"").append(ruleOptions->guiIconName).append("\";\n");
    }
    str.append("ruleOptions[R_").append(ruleName.toUpper()).append("] = ").append(varName).append(";");

    return str;
}

QString BNFRule::toString() const
{
    QString str;

    BNFRuleItem *item;
    str.append(ruleName);
    //if(!ruleOptions.isEmpty()){
    //    str.append(" {").append(ruleOptions).append("}");
    //}
    str.append(" : ");
    for(int i=0; i<alternatives.size(); ++i){
        if(i>0){
            str.append(" | ");
        }
        QList<BNFRuleItem*> items=alternatives.at(i);
        for(int k=0; k<items.size(); ++k){
            item=items.at(k);

            if(item->isTerminal){
                str.append(" ");
                if(item->token.isLiteralTerminal){str.append("'");}
                str.append(item->token.tokenType==EBNFToken::EPSILON ? "Epsilon" : item->token.lexeme);
                if(item->token.isLiteralTerminal){str.append("'");}
            }else{
                str.append(" ");
                if(item->isNegated){
                    str.append("~");
                }
                str.append(item->pointsTo);
            }
        }
    }
    str.append(" ;");

    return str;
}
