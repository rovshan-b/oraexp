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

ParsingTableBuilder::ParsingTableBuilder(DFA *dfa,
                                         const QStringList &keywords,
                                         const QList<EBNFToken> &tokens,
                                         int eofTokenId) :
    dfa(dfa),
    keywords(keywords),
    tokens(tokens),
    eofTokenId(eofTokenId)
{
    QTime time;
    time.start();

    createParsingTable();
    //printoutParsingTable();
    compactParsingTable();
    printoutParsingTable();

    qDebug() << "constructed parsing table in" << time.elapsed() << "ms";

    printoutForTargetParser();
}

ParsingTableBuilder::~ParsingTableBuilder()
{
    qDeleteAll(tableRows);
}

void ParsingTableBuilder::createParsingTable()
{
    EBNFToken eofToken=EBNFScanner::createEOFToken();
    //eofToken.

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
                    Q_ASSERT(termDefId>=NON_LITERAL_START_IX);
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
                int keywordIx=eofTokenId;
                row->addAction(keywordIx, ParsingTableAction::Accept, -1);

                continue;
            }

            QList<EBNFToken> lookaheads=state->lookaheads.value(dfaItem);
            Q_ASSERT(lookaheads.size()>0);
            for(int l=0; l<lookaheads.size(); ++l){
                const EBNFToken &lookahead=lookaheads.at(l);
                int keywordIx=lookahead.isLiteralTerminal ? getKeywordIx(lookahead.lexeme) : lookahead.nonLiteralTerminalDefId;
                Q_ASSERT(keywordIx>=0);
                row->addAction(keywordIx, ParsingTableAction::Reduce, dfaItem->rule->ruleDefId, dfaItem->getSymbolCount());
            }
        }

        //row->sortActions();
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

    int ix = it-keywords.begin();

    Q_ASSERT(ix>=0);

    return ix;
}

void ParsingTableBuilder::printoutParsingTable() const
{
    qDebug("------------Parsing table-------------");

    for(int i=0; i<tableRows.size(); ++i){
        printoutTableRow(tableRows.at(i));
    }
}

void ParsingTableBuilder::printoutTableRow(ParsingTableRow* row) const
{
    qDebug() << "State" << row->stateId;
    qDebug() << "   Actions:";
    QList<int> actionKeys=row->actions->keys();
    for(int k=0; k<actionKeys.size(); ++k){
        ParsingTableAction* action=row->actions->value(actionKeys.at(k));
        int key = actionKeys.at(k);
        QString keyDesc = key >= NON_LITERAL_START_IX ? this->tokens.at(key-NON_LITERAL_START_IX).lexeme : this->keywords.at(key);
        qDebug() << "      on " << keyDesc
                 << action->getActionTypeAsString()
                 << action->stateOrRuleId
                 << (action->actionType==ParsingTableAction::Reduce ? qPrintable(QString("(rb %1)").arg(QString::number(action->symbolCount))) :
                                                                      qPrintable(QString("")));
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

void ParsingTableBuilder::printoutForTargetParser()
{
    qDebug("----------------parsing table code for target parser---------------");

    //first print out individual actions
    for(int i=0; i<tableRows.size(); ++i){
        ParsingTableRow *row=tableRows.at(i);

        QList<int> actionKeys=row->actions->keys();
        for(int k=0; k<actionKeys.size(); ++k){
            int key=actionKeys.at(k);
            ParsingTableAction *action=row->actions->value(key, 0);
            Q_ASSERT(action);

            if(actionVarNames.value(action)!=0){ //already has generated action
                continue;
            }

            QString varName=QString("action_%1_%2").arg(row->stateId).arg(getTokenName(key));
            actionVarNames[action]=varName;
            printoutActionCode(varName, action);
        }
    }

    //now print out declarations for table rows
    //qDebug("QList<ParsingTableRow*> rows;");
    qDebug() << "rows.reserve(" << tableRows.size() << ");\n";

    for(int i=0; i<tableRows.size(); ++i){
        ParsingTableRow *row=tableRows.at(i);

        printoutRowCode(row);
    }

    qDebug("--------------end parsing table code for target parser-------------");
}

void ParsingTableBuilder::printoutActionCode(const QString &varName, ParsingTableAction *action) const
{
    QString decl=QString("ParsingTableAction * %1 = new ParsingTableAction();\n").arg(varName);
    decl.append(QString("%1->actionType = ParsingTableAction::%2;\n").arg(varName).arg(action->getActionTypeAsString()));
    decl.append(QString("%1->stateOrRuleId = %2;\n").arg(varName).arg(QString::number(action->stateOrRuleId)));
    decl.append(QString("%1->symbolCount = %2;\n").arg(varName).arg(QString::number(action->symbolCount)));

    qDebug() << qPrintable(decl);
}

void ParsingTableBuilder::printoutRowCode(ParsingTableRow *row) const
{
    QString varName=QString("row_%1").arg(row->stateId);
    QString decl=QString("ParsingTableRow *%1 = new ParsingTableRow();\n").arg(varName);
    decl.append(QString("%1->stateId = %2;\n").arg(varName).arg(row->stateId));
    decl.append(QString("%1->ownsActions = %2;\n").arg(varName).arg(row->ownsActions ? "true" : "false"));
    QList<int> actionKeys=row->actions->keys();
    for(int k=0; k<actionKeys.size(); ++k){
        int key=actionKeys.at(k);
        ParsingTableAction *action=row->actions->value(key, 0);
        Q_ASSERT(action);

        QString actionVarName=actionVarNames.value(action);
        Q_ASSERT(!actionVarName.isEmpty());

        decl.append(QString("(*%1->actions)[%2] = %3;\n").arg(varName).arg(getTokenName(key)).arg(actionVarName));
    }

    QList<int> gotoKeys=row->gotos.keys();
    for(int m=0; m<gotoKeys.size(); ++m){
        int key=gotoKeys.at(m);

        decl.append(QString("%1->gotos[%2] = %3;\n").arg(varName).arg(key).arg(row->gotos.value(key)));
    }

    decl.append(QString("rows.append(%1);\n").arg(varName));

    qDebug() << qPrintable(decl);
}

QString ParsingTableBuilder::getTokenName(int tokenId) const
{
    if(tokenId<NON_LITERAL_START_IX){ //it is keyword
        return QString::number(tokenId);
    }

    int listIx = tokenId - NON_LITERAL_START_IX;

    Q_ASSERT(listIx>=0 && listIx<tokens.size());

    return tokens.at(listIx).lexeme;
}
