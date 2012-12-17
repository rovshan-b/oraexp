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
                ParsingTableAction *action=new ParsingTableAction();
                action->actionType=ParsingTableAction::Shift;
                action->stateOrRuleId=transition->targetState->stateId;

                if(ruleItem->token.isLiteralTerminal){
                    int keywordIx=getKeywordIx(ruleItem->token.lexeme);
                    if(!row->actions->contains(keywordIx)){ //met transition to same state on same terminal from different dfa items
                        (*row->actions)[keywordIx]=action;
                    }else{
                        delete action;
                    }
                }else{
                    int termDefId=ruleItem->token.nonLiteralTerminalDefId;
                    if(!row->actions->contains(termDefId)){
                        (*row->actions)[termDefId]=action;
                    }else{
                        delete action;
                    }

                }
            }else{
                int ruleDefId=EBNFParser::findRuleByName(dfa->bnfRules, ruleItem->pointsTo)->ruleDefId;
                Q_ASSERT(ruleDefId<=dfa->bnfRules.size());
                row->gotos[ruleDefId]=transition->targetState->stateId;
            }
        }

        for(int j=0; j<state->dfaItems.size(); ++j){
            DFAItem *dfaItem=state->dfaItems.at(j);
            if(!dfaItem->isCompleteItem()){ //populated above using transitions
                continue;
            }

            if(dfaItem->rule->ruleDefId==1 &&
                    state->lookaheads.value(dfaItem).contains(eofToken)){ //there can be only one complete item with start rule (accepting rule)
                ParsingTableAction *action=new ParsingTableAction();
                action->actionType=ParsingTableAction::Accept;
                int keywordIx=getKeywordIx(eofToken.lexeme);
                Q_ASSERT(!(*row->actions).contains(keywordIx));
                (*row->actions)[keywordIx]=action;

                continue;
            }

            QList<EBNFToken> lookaheads=state->lookaheads.value(dfaItem);
            Q_ASSERT(lookaheads.size()>0);
            for(int l=0; l<lookaheads.size(); ++l){
                ParsingTableAction *action=new ParsingTableAction();
                action->actionType=ParsingTableAction::Reduce;
                action->stateOrRuleId=dfaItem->rule->ruleDefId;
                int keywordIx=getKeywordIx(lookaheads.at(l).lexeme);
                if(!row->actions->contains(keywordIx)){
                    (*row->actions)[keywordIx]=action;
                }else{
                    delete action;
                }
            }
        }
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

                qDeleteAll(rowToCompare->actions->values());
                delete rowToCompare->actions;
                rowToCompare->actions=row->actions;
                rowToCompare->ownsActions=false;

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
        ParsingTableRow *row=tableRows.at(i);
        qDebug() << "State" << row->stateId;
        qDebug() << "   Actions:";
        QList<int> actionKeys=row->actions->keys();
        for(int k=0; k<actionKeys.size(); ++k){
            ParsingTableAction *action=row->actions->value(actionKeys.at(k));
            qDebug() << "      on " << keywords.at(actionKeys.at(k)) << action->getActionTypeAsString()
                     << action->stateOrRuleId;
        }

        qDebug() << "   Gotos:";
        QList<int> gotoKeys=row->gotos.keys();
        for(int l=0; l<gotoKeys.size(); ++l){
            int ruleDefId=gotoKeys.at(l);
            int stateId=row->gotos.value(ruleDefId);
            Q_ASSERT(ruleDefId!=-1);
            qDebug() << "      on " << dfa->bnfRules.at(ruleDefId-1)->ruleName << stateId;
        }
    }
}
