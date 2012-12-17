#ifndef EBNFPARSER_H
#define EBNFPARSER_H

#include <QHash>
#include <QStringList>
#include "ebnftoken.h"

class BNFRule;
class EBNFScanner;
class BNFRuleItem;

class EBNFParser
{
public:
    EBNFParser();
    ~EBNFParser();

    void parse();

    QList<BNFRule*> getBNFRules() const;
    bool getHasMissingRules() const {return this->hasMissingRules;}
    QStringList getTargetScannerKeywords() const {return this->targetScannerKeywords;}

    static BNFRule *findRuleByName(const QList<BNFRule*> &bnfRules, const QString &ruleName);

private:
    void doParse();
    BNFRule *createRule(const QString &ruleName);
    void error(const QString &msg);
    bool match(EBNFToken::EBNFTokenType tokenType);
    BNFRule *rule();
    void exp(BNFRule *rule);
    bool factor(BNFRule *rule);
    bool atom(BNFRule *rule);

    BNFRuleItem *createEpsilonItem();
    BNFRuleItem *createRulePointerItem(BNFRule *ruleToPointTo);
    BNFRuleItem *convertToKleeneItem(const QString &newRuleName, BNFRuleItem *item);
    BNFRuleItem *convertToOneOrMoreItem(const QString &newRuleName, BNFRuleItem *item);
    BNFRuleItem *convertToZeroOrOneItem(const QString &newRuleName, BNFRuleItem *item);

    void findMissingRuleDefinitions();
    void printoutRules();
    void registerTargetScannerToken(EBNFToken &token);
    void registerTargetScannerKeyword(EBNFToken &token);
    void registerTargetScannerKeywordsFromFile();
    void printTargetScannerTokens();
    void printTargetScannerKeywords();

    EBNFScanner *scanner;
    EBNFToken token;
    QList<BNFRule*> rules;
    QStringList targetScannerTokens;
    QStringList targetScannerKeywords;

    bool hasMissingRules;
};

#endif // EBNFPARSER_H
