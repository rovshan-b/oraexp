#ifndef EBNFPARSER_H
#define EBNFPARSER_H

#include <QHash>
#include <QStringList>
#include "ebnftoken.h"
#include "filewriter.h"

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
    QList<EBNFToken> getTargetScannerTokens() const {return this->targetScannerTokens;}

    static BNFRule *findRuleByName(const QList<BNFRule*> &bnfRules, const QString &ruleName);

    int eofTokenId;

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
    void registerTargetScannerKeywordsFromFile(const QString &filename, QStringList &collection);
    void printTargetScannerTokens();
    void printTargetScannerKeywords(const QString &varName, const QStringList &collection, FileWriter::Destination destination);

    EBNFScanner *scanner;
    EBNFToken token;
    QList<BNFRule*> rules;
    QList<EBNFToken> targetScannerTokens;
    QStringList targetScannerKeywords;
    QStringList reservedWords;

    bool hasMissingRules;
};

#endif // EBNFPARSER_H
