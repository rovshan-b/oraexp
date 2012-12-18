#include "parsingtablebuilder.h"
#include "dfa.h"
#include "dfastate.h"
#include "dfatransition.h"
#include "dfaitem.h"
#include "bnfrule.h"
#include "bnfruleitem.h"
#include "ebnfscanner.h"
#include "ebnfparser.h"
#include <QTime>
#include <QDebug>

ParsingTableBuilder::ParsingTableBuilder(DFA *dfa, const QStringList &keywords) :
    dfa(dfa),
    keywords(keywords)
{
    QTime time;
    time.start();

    createParsingTable();
    //printoutParsingTable();
    compactParsingTable();
    printoutParsingTable();

    qDebug() << "constructed parsing table in" << time.elapsed() << "ms";
}

ParsingTableBuilder::~ParsingTableBuilder()
{
    qDeleteAll(tableRows);
}

void ParsingTableBuilder::createParsingTable()
{
    EBNFToken eofToken=EBNFScanner::createEOFToken();

    for(int i=0; i<dfa->states.size(); ++i){
        DFAState *state=dfa->states.at(i);

        ParsingTableRow* row=new ParsingTableRow();
        row->stateId=i;
        tableRows.append(row);

        for(int k=0; k<state->transitions.size(); ++k){
            DFATransition *transition=state->transitions.at(k);
            BNFRuleItem *ruleItem=transition->sourceItem->currentRuleItem();

            if(ruleItem->isTerminal){
                if(ruleItem->token.isLiteralTerminal){
                    int keywordIx=getKeywordIx(ruleItem->token.lexeme);
                    row->addAction(keywordIx, ParsingTableAction::Shift, transition->targetState->stateId);
                }else{
                    int termDefId=ruleItem->token.nonLiteralTerminalDefId;
                    row->addAction(termDefId, ParsingTableAction::Shift, transition->targetState->stateId);

                }
            }else{
                int ruleDefId=EBNFParser::findRuleByName(dfa->bnfRules, ruleItem->pointsTo)->ruleDefId;
                Q_ASSERT(ruleDefId<=dfa->bnfRules.size());
                row->addGoto(ruleDefId, transition->targetState->stateId);
            }
        }

        for(int j=0; j<state->dfaItems.size(); ++j){
            DFAItem *dfaItem=state->dfaItems.at(j);
            if(!dfaItem->isCompleteItem()){ //populated above using transitions
                continue;
            }

            if(dfaItem->rule->ruleDefId==1 &&
                    state->lookaheads.value(dfaItem).contains(eofToken)){ //there can be only one complete item with start rule (accepting rule)
                int keywordIx=getKeywordIx(eofToken.lexeme);
                row->addAction(keywordIx, ParsingTableAction::Accept, -1);

                continue;
            }

            QList<EBNFToken> lookaheads=state->lookaheads.value(dfaItem);
            Q_ASSERT(lookaheads.size()>0);
            for(int l=0; l<lookaheads.size(); ++l){
                int keywordIx=getKeywordIx(lookaheads.at(l).lexeme);
                row->addAction(keywordIx, ParsingTableAction::Reduce, dfaItem->rule->ruleDefId, dfaItem->getSymbolCount());
            }
        }

        row->sortActions();
    }
}

void ParsingTableBuilder::compactParsingTable()
{
    int deletedActionRows=0;
    //compact by finding and merging equal action row
    for(int i=0; i<tableRows.size(); ++i){
        ParsingTableRow* row=tableRows.at(i);

        for(int k=i+1; k<tableRows.size(); ++k){
            ParsingTableRow* rowToCompare=tableRows.at(k);

            if(rowToCompare->ownsActions && row->hasEqualActions(rowToCompare)){
                qDebug() << "replacing actions of state" << rowToCompare->stateId << "to actions of state" << row->stateId;

                rowToCompare->replaceActionsWith(row);

                ++deletedActionRows;
            }
        }
    }

    qDebug() << "deleted" << deletedActionRows << "action row(s) while compacting parsing table";
}

bool caseInsensitiveLessThan(const QString &s1, const QString &s2)
{
    return s1.toUpper() < s2.toUpper();
}

int ParsingTableBuilder::getKeywordIx(const QString &keyword) const
{
    QStringList::const_iterator it=qBinaryFind(keywords.begin(),
                                               keywords.end(),
                                               keyword,
                                               caseInsensitiveLessThan);
    Q_ASSERT(it!=keywords.end());

    return it-keywords.begin();
}

void ParsingTableBuilder::printoutParsingTable() const
{
    qDebug("------------Parsing table-------------");

    for(int i=0; i<tableRows.size(); ++i){
        tableRows.at(i)->printout(dfa, &keywords);
    }
}
