#include "ebnfparser.h"
#include "ebnfscanner.h"
#include "bnfrule.h"
#include "bnfruleitem.h"
#include <QFile>
#include <QTextStream>
#include <stdlib.h>
#include <QTime>
#include <QtDebug>

EBNFParser::EBNFParser() : scanner(0), hasMissingRules(false)
{

}

EBNFParser::~EBNFParser()
{
    if(scanner!=0){
        delete scanner;
    }
}

void EBNFParser::parse()
{
    QTime time;
    time.start();

    QFile grammarFile("/home/rovshan/Projects/Qt/OraExp/grammars/pl_sql");
    if(grammarFile.open(QFile::ReadOnly)){
        QTextStream strm(&grammarFile);
        scanner=new EBNFScanner(&strm);
        doParse();
    }

    qDebug() << "parsed ebnf rules in" << time.elapsed() << "ms";
    qDebug() << "rule count:" << rules.size();

    EBNFToken errToken;
    errToken.tokenType=EBNFToken::ERR;
    errToken.lexeme="PLS_ERR";
    errToken.isLiteralTerminal=false;
    registerTargetScannerToken(errToken);

    EBNFToken eofToken=EBNFScanner::createEOFToken();
    eofToken.lexeme="PLS_E_O_F";
    registerTargetScannerToken(eofToken);

    printTargetScannerTokens();

    //registerTargetScannerKeywordsFromFile();
    //EBNFToken eofToken=EBNFScanner::createEOFToken();
    //registerTargetScannerKeyword(eofToken);
    targetScannerKeywords.sort();
    printTargetScannerKeywords();

    time.restart();
    findMissingRuleDefinitions();
    qDebug() << "checked rules in" << time.elapsed() << "ms";

    qDebug() << "---------rules----------";
    printoutRules();
    qDebug() << "-------end rules--------";
}

void EBNFParser::doParse()
{
    qDeleteAll(rules);
    rules.clear();

    hasMissingRules=false;

    token=scanner->getToken();
    while(token.tokenType!=EBNFToken::E_O_F &&
          token.tokenType!=EBNFToken::ERR){

        BNFRule *r=rule();
        if(r==0){
            return;
        }
    }
}

BNFRule *EBNFParser::createRule(const QString &ruleName)
{
    BNFRule *r=new BNFRule();
    r->ruleName=ruleName;
    rules.append(r);
    return r;
}

void EBNFParser::error(const QString &msg)
{
    qDebug() << "Error in EBNFParser -" << msg;
    if(scanner!=0){
        qDebug() << "Current scanner position:" << scanner->getCurrPos()
                 << ", current line:" << scanner->getCurrLineNo()
                 << ", line position:" << scanner->getCurrLinePos();
    }
    exit(1);
}

bool EBNFParser::match(EBNFToken::EBNFTokenType tokenType)
{
    if(token.tokenType!=tokenType){
        error(QString("Expected %1 but encountered %2 at position %3 - %4").arg(tokenType).arg(token.tokenType).arg(token.startPos).arg(token.endPos));
        return false;
    }else{
        token=scanner->getToken();

        if(token.tokenType==EBNFToken::TERMINAL){
            if(!token.isLiteralTerminal){
                registerTargetScannerToken(token);
            }else{
                registerTargetScannerKeyword(token);
            }
        }

        return true;
    }
}

/*
#rulelist -> rule rulelist
rule -> name : exp ;
exp -> factor | exp
factor -> atom factor
atom -> negation ( exp ) rep | negation identifier rep | negation 'terminal' rep
negation -> ~ | epsilon
rep -> * | + | ? | epsilon
*/

BNFRule *EBNFParser::rule()
{
    //match name
    if(token.tokenType!=EBNFToken::ID){
        error("Rule must start with name");
        return 0;
    }

    BNFRule *r=createRule(token.lexeme);
    r->startAlternatives();

    match(EBNFToken::ID);
    match(EBNFToken::COLON);

    exp(r);
    match(EBNFToken::SEMICOLON);

    if(r->alternatives.size()>0 && r->alternatives.at(r->alternatives.size()-1).isEmpty()){
        r->alternatives[r->alternatives.size()-1].append(createEpsilonItem());
    }

    return r;
}

void EBNFParser::exp(BNFRule *rule)
{
    bool matched=factor(rule);
    if(matched && token.tokenType==EBNFToken::PIPE){

        rule->startAlternatives();

        match(EBNFToken::PIPE);
        exp(rule);
    }
}

bool EBNFParser::factor(BNFRule *rule)
{
    bool matched=atom(rule);
    if(matched){
        factor(rule);
    }
    return matched;
}

bool EBNFParser::atom(BNFRule *rule)
{
    if(token.tokenType==EBNFToken::RPAREN ||
            token.tokenType==EBNFToken::SEMICOLON ||
            token.tokenType==EBNFToken::PIPE){
        return false;
    }

    BNFRuleItem *item=new BNFRuleItem();

    if(token.tokenType==EBNFToken::NEGATION){
        item->isNegated=true;
        match(EBNFToken::NEGATION);
    }

    if(token.tokenType==EBNFToken::LPAREN){
        //Convert every group ( E ) to a fresh non-terminal X and add
        //X = E
        match(EBNFToken::LPAREN);
        BNFRule *r=createRule(QString("%1_%2").arg(rule->ruleName).arg(++rule->subruleCount));
        r->startAlternatives();
        item->isTerminal=false;
        item->pointsTo=r->ruleName;
        item->token=token;
        exp(r);
        match(EBNFToken::RPAREN);
    }else if(token.tokenType==EBNFToken::ID){
        item->isTerminal=false;
        item->pointsTo=token.lexeme;
        item->token=token;
        match(EBNFToken::ID);
    }else if(token.tokenType==EBNFToken::TERMINAL){
        item->isTerminal=true;
        item->token=token;
        match(EBNFToken::TERMINAL);
    }else{
        error("Could not match atom");
    }

    if(token.tokenType==EBNFToken::RANGE){
        if(item->token.tokenType!=EBNFToken::TERMINAL){
            error("Range start must be a terminal");
        }
        match(EBNFToken::RANGE);
        if(token.tokenType!=EBNFToken::TERMINAL){
            error("Range end must be a terminal");
        }
        item->rangeEndToken=token;
        match(EBNFToken::TERMINAL);
    }else if(token.tokenType==EBNFToken::STAR){
        item=convertToKleeneItem(QString("%1_%2_kleene").arg(rule->ruleName).arg(++rule->subruleCount), item);
        match(EBNFToken::STAR);
    }else if(token.tokenType==EBNFToken::PLUS){
        item=convertToOneOrMoreItem(QString("%1_%2_plus").arg(rule->ruleName).arg(++rule->subruleCount), item);
        match(EBNFToken::PLUS);
    }else if(token.tokenType==EBNFToken::QUESTION){
        item=convertToZeroOrOneItem(QString("%1_%2_question").arg(rule->ruleName).arg(++rule->subruleCount), item);
        match(EBNFToken::QUESTION);
    }

    rule->addRuleItem(item);

    return true;
}

BNFRuleItem *EBNFParser::createEpsilonItem()
{
    BNFRuleItem *epsilon=new BNFRuleItem();
    epsilon->isTerminal=true;
    epsilon->token.tokenType=EBNFToken::EPSILON;

    return epsilon;
}

BNFRuleItem *EBNFParser::createRulePointerItem(BNFRule *ruleToPointTo)
{
    BNFRuleItem *rulePointerItem=new BNFRuleItem();
    rulePointerItem->isTerminal=false;
    rulePointerItem->pointsTo=ruleToPointTo->ruleName;
    rulePointerItem->token.tokenType=EBNFToken::ID;
    rulePointerItem->token.lexeme=ruleToPointTo->ruleName;

    return rulePointerItem;
}

BNFRuleItem *EBNFParser::convertToKleeneItem(const QString &newRuleName, BNFRuleItem *item)
{
    //Convert every E* to a fresh non-terminal X and add
    //X = epsilon | X E.

    //create X
    BNFRule *r=createRule(newRuleName);
    r->startAlternatives();

    //add epsilon
    r->addRuleItem(createEpsilonItem());

    //start new alternatives section
    r->startAlternatives();

    //add pointer to X
    r->addRuleItem(createRulePointerItem(r));
    //add pointer to E
    item->token.lexeme=item->pointsTo;
    r->addRuleItem(item);

    //return item to point to X
    return createRulePointerItem(r);
}

BNFRuleItem *EBNFParser::convertToOneOrMoreItem(const QString &newRuleName, BNFRuleItem *item)
{
    //BNF can express 1 or more repetitions by e.g.:
    //X = E | X E

    //create X
    BNFRule *r=createRule(newRuleName);
    r->startAlternatives();

    //add pointer to copy of E
    item->token.lexeme=item->pointsTo;
    r->addRuleItem(new BNFRuleItem(*item));

    //start new alternatives section
    r->startAlternatives();

    //add pointer to X
    r->addRuleItem(createRulePointerItem(r));
    //add pointer to E
    r->addRuleItem(item);

    //return item to point to X
    return createRulePointerItem(r);
}

BNFRuleItem *EBNFParser::convertToZeroOrOneItem(const QString &newRuleName, BNFRuleItem *item)
{
    //Convert every option E? to a fresh non-terminal X and add
    //X = epsilon | E.

    //create X
    BNFRule *r=createRule(newRuleName);

    r->startAlternatives();

    //add epsilon
    r->addRuleItem(createEpsilonItem());

    //start new alternatives section
    r->startAlternatives();

    //add pointer to E
    item->token.lexeme=item->pointsTo;
    r->addRuleItem(item);

    //return item to point to X
    return createRulePointerItem(r);
}

void EBNFParser::findMissingRuleDefinitions()
{
    QList<QString> searchedItemNames;
    BNFRule *r;
    BNFRuleItem *item;

    if(rules.size()>0){
        rules.at(0)->isReachableFromStartSymbol=true;
    }

    for(int i=0; i<rules.size(); ++i){
        r=rules.at(i);

        for(int i=0; i<r->alternatives.size(); ++i){
            for(int k=0; k<r->alternatives.at(i).size(); ++k){

                item=r->alternatives.at(i).at(k);
                if(item->isTerminal){continue;}

                if(!searchedItemNames.contains(item->pointsTo)){
                    searchedItemNames.append(item->pointsTo);
                }else{
                    continue;
                }

                bool found=false;
                for(int j=0; j<rules.size(); ++j){
                    if(rules.at(j)->ruleName==item->pointsTo){
                        found=true;
                        rules.at(j)->isReachableFromStartSymbol=true;
                        break;
                    }
                }

                if(!found){
                    qDebug() << "Definition not found for rule named" << qPrintable(item->pointsTo);
                    hasMissingRules=true;
                }
            }
        }
    }

    for(int i = 1; i<rules.size(); ++i){
        r=rules.at(i);
        if(!r->isReachableFromStartSymbol){
            qDebug() << "Rule not reachable from start symbol -" << qPrintable(r->ruleName);
        }
    }
}

void EBNFParser::printoutRules(){
    for(int i=0; i<rules.size(); ++i){
        qDebug() << qPrintable(rules.at(i)->toString());
    }
}

void EBNFParser::registerTargetScannerToken(EBNFToken &token)
{
    int ix=targetScannerTokens.indexOf(token);
    if(ix==-1){
        token.nonLiteralTerminalDefId=NON_LITERAL_START_IX+targetScannerTokens.size();
        targetScannerTokens.append(token);

        if(token.tokenType==EBNFToken::E_O_F){
            this->eofTokenId=token.nonLiteralTerminalDefId;
        }
    }else{
        token.nonLiteralTerminalDefId=targetScannerTokens.at(ix).nonLiteralTerminalDefId;
    }
}

void EBNFParser::registerTargetScannerKeyword(EBNFToken &token)
{
    if(!targetScannerKeywords.contains(token.lexeme)){
        targetScannerKeywords.append(token.lexeme);
    }
}

void EBNFParser::registerTargetScannerKeywordsFromFile()
{
    QFile keywordsFile("/home/rovshan/Projects/Qt/OraExp/misc/keywords");
    bool opened=keywordsFile.open(QIODevice::ReadOnly);
    Q_ASSERT(opened);
    QTextStream in(&keywordsFile);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if(!line.isEmpty()){
            if(!targetScannerKeywords.contains(line)){
                targetScannerKeywords.append(line);
            }
        }
    }
}

void EBNFParser::printTargetScannerTokens()
{
    qDebug("---------target scanner tokens------------");

    foreach(const EBNFToken &token, targetScannerTokens){
        qDebug() << "#define" << qPrintable(token.lexeme) << token.nonLiteralTerminalDefId;
    }

    qDebug("-------end target scanner tokens----------");
}

void EBNFParser::printTargetScannerKeywords()
{
    qDebug("---------target scanner keywords------------");

    if(targetScannerKeywords.size()>0){
        qDebug("QStringList keywords;");
        qDebug() << qPrintable(QString("keywords.reserve(%1);").arg(targetScannerKeywords.size()));
    }
    QString line;

    foreach(const QString &keyword, targetScannerKeywords){
        line = QString("keywords.append(\"%1\");").arg(keyword);
        qDebug() << qPrintable(line);
    }

    qDebug("-------end target scanner keywords----------");
}

QList<BNFRule*> EBNFParser::getBNFRules() const
{
    return this->rules;
}

BNFRule *EBNFParser::findRuleByName(const QList<BNFRule *> &bnfRules, const QString &ruleName)
{
    for(int i=0; i<bnfRules.size(); ++i){
        if(ruleName==bnfRules.at(i)->ruleName){
            return bnfRules.at(i);
        }
    }

    Q_ASSERT(false);

    return 0;
}
