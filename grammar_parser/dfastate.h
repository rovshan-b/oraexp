#ifndef DFASTATE_H
#define DFASTATE_H

#include <QList>
#include <QHash>
#include "ebnftoken.h"

class DFAItem;
class DFATransition;
class BNFRuleItem;

class DFAState
{
public:
    DFAState();
    ~DFAState();

    bool operator==(const DFAState &other) const;
    bool equalsByItems(QList<DFAItem*> items) const;

    int stateId;
    QList<DFAItem*> dfaItems;
    QList<DFATransition*> transitions;

    QHash<DFAItem*, QList<EBNFToken> > lookaheads;

    void addItem(DFAItem *item, bool kernelItem);

    bool contains(DFAItem *item) const;

    int kernelItemCount() const;
    bool isKernelItem(DFAItem *item) const;
    bool containsKernelItem(DFAItem *item) const;
    bool containsKernelItems(QList<DFAItem*> items) const;
    bool equalsByKernelItems(DFAState *otherState) const;

    void addTransition(DFATransition *transition);
    DFATransition *findTransitionOnDFAItem(DFAItem *dfaItem);

    bool addLookahead(DFAItem *item, const EBNFToken &lookahead);

    QString toString() const;

private:
    QList<int> kernelItemIndexes;
};

#endif // DFASTATE_H
