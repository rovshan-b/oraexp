#ifndef DFA_H
#define DFA_H

#include <QList>

class BNFRule;
class BNFRuleItem;
class DFAItem;
class DFAState;
class DFATransition;

class DFA
{
public:
    DFA(const QList<BNFRule*> &bnfRules);
    ~DFA();

private:
    QList<BNFRule*> bnfRules;
    QList<DFAItem*> dfaItems;
    QList<DFAState*> states;

    DFAState *createDFAState();
    void augmentStartRule();
    void generateDFAItems();
    void constructDFAforLR0();
    void computeTransitions(DFAState *state);

    QList<DFAItem*> closure(QList<DFAItem*> items) const;
    QList<DFAItem*> go_to(QList<DFAItem*> items, BNFRuleItem *ruleItem);
    void canonicalCollection();

    void constructDFAforLALR1();

    QList<DFAItem*> findAllInitialDFAItemsForRule(const QString &ruleName) const;
    DFAItem *findDFAItem(BNFRule *rule, int altIx, int position) const;
    DFAItem *findNextDFAItem(DFAItem *currItem) const;
    DFAState* hasStateWithItems(QList<DFAItem*> items) const;

    void printoutDFA();

    int stateCounter;
};

#endif // DFA_H
