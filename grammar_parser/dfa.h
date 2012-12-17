#ifndef DFA_H
#define DFA_H

#include <QList>
#include <QHash>
#include <QPair>
#include "ebnftoken.h"

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

    QList<BNFRule*> bnfRules;
    QList<DFAItem*> dfaItems;
    QList<DFAState*> states;

private:
    DFAState *createDFAState();
    void augmentStartRule();
    void generateDFAItems();
    void constructDFAforLR0();
    void computeTransitions(DFAState *state);
    void checkTransitions();

    QList<DFAItem*> closure(QList<DFAItem*> items) const;
    QList<DFAItem*> go_to(QList<DFAItem*> items, BNFRuleItem *ruleItem);
    void canonicalCollection();

    void constructDFAforLALR1();
    void computeLookaheadPropagations();
    void setLookaheadPropagations(DFAState *state, DFAState *tmpState, DFAItem *keyItem);
    void propagateLookaheads();
    void closeItems();
    DFAState *createTmpStateWithNonGrammarSymbol(DFAItem *item) const;
    void closure_lalr1(DFAState *state) const;
    void checkForConflicts();
    bool hasIntersectingLookaheads(DFAState *state, DFAItem *first, DFAItem *second) const;

    QList<DFAItem*> findAllInitialDFAItemsForRule(const QString &ruleName) const;
    DFAItem *findDFAItem(BNFRule *rule, int altIx, int position) const;
    DFAItem *findNextDFAItem(DFAItem *currItem) const;
    DFAState* hasStateWithItems(QList<DFAItem*> items) const;
    QList<DFAState*> findAllStatesWithDFAItem(DFAItem *dfaItem) const;

    EBNFToken createNonGrammarToken() const;

    void printoutTargetParserRules();
    void printoutLookaheadsPropagationTable();
    void printoutDFA();
    void printoutState(DFAState *state);

    int stateCounter;

    QHash< QPair<DFAState*, DFAItem*>,
           QList< QPair<DFAState*, DFAItem* > > > lookaheadsPropagationTable;
};

#endif // DFA_H
