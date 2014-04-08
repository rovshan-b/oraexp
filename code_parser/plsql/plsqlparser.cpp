#include "plsqlparser.h"
#include "plsqlparsingtable.h"
#include "beans/tokeninfo.h"

PlSqlParser::PlSqlParser(CodeScanner *scanner) : CodeParser(scanner)
{
}

ParsingTable *PlSqlParser::getParsingTable()
{
    return PlSqlParsingTable::getInstance();
}

void PlSqlParser::correctError(int *token, ParsingTableRow *row, ParsingTableAction **actionOnCurrentToken)
{
    //qDebug("--------started error recovery--------------");
    bool reservedWord = parsingTable->isReservedWord(scanner->getTokenLexeme());

    if(*token!=PLS_SEMI){
        *actionOnCurrentToken=row->actions->value(PLS_NOT_SEMI, 0);

        if(!reservedWord){
            replaceKeywordWithIdentifier(*token, row, actionOnCurrentToken);
        }
    }

    if(*actionOnCurrentToken == 0){ //try to recognize major constructs
        //read input until we encounter one of (first of) PLS_SEMI, END opt_identifier PLS_SEMI
        //while reading add all read tokens to token stack
        QList<TokenInfo*> reduceTokens;
        do{
            if(!reservedWord){
                reduceTokens.prepend(createTokenInfo(*token));
            }

            if(*token == PLS_SEMI || reservedWord){
                if(reduceMajorConstruct(reduceTokens)){
                    if(!reservedWord){
                        //read next token for parser to operate on
                        *token = scanner->getNextToken();
                    }
                    ParsingTableRow *newRow=parsingTable->rows.at(stack.top()); //reduceMajorConstruct pushed new gotoState into stack
                    *actionOnCurrentToken=(*newRow->actions).value(*token, 0);

                    replaceKeywordWithIdentifier(*token, newRow, actionOnCurrentToken);
                }else{
                    qDeleteAll(reduceTokens);
                }
                break;
            }

            *token = scanner->getNextToken();
            reservedWord = parsingTable->isReservedWord(scanner->getTokenLexeme());
        }while(*token != PLS_E_O_F);
    }

        //qDebug("--------completed error recovery--------------");
}

void PlSqlParser::replaceKeywordWithIdentifier(int token, ParsingTableRow *row, ParsingTableAction **actionOnCurrentToken)
{
    if(*actionOnCurrentToken == 0 && token < NON_LITERAL_START_IX){ //check if keyword is used in place of identifier
        *actionOnCurrentToken=row->actions->value(PLS_ID, 0);
    }
}

bool PlSqlParser::reduceMajorConstruct(QList<TokenInfo*> &reduceTokens)
{
    const int majorConstructCount = 3;
    const int majorConstructs[] = {R_EXPRESSION, R_DECLARATION, R_STATEMENT};

    ParsingTable *table=getParsingTable();
    int gotoState;
    int currentConstruct;

    while(!stack.isEmpty()){ //find first state when we can do some action on provided token
        int stateOnTop = stack.top();

        ParsingTableRow *row=table->rows.at(stateOnTop);
        Q_ASSERT(row);

        for(int i=0; i<majorConstructCount; ++i){
            currentConstruct = majorConstructs[i];
            gotoState=row->gotos.value(currentConstruct, -1);
            if(gotoState != -1){
                break;
            }
        }

        if(gotoState != -1){
            reduce(currentConstruct, 0, reduceTokens);
            return true;
        }else{
            stack.pop();
            reduceTokens.append(tokenStack.pop()); //append popped items to reduce tokens so that they get added to parse tree
        }
    }

    return false;
}
/*
bool PlSqlParser::restoreParsingFromNextState(int *token, QList<TokenInfo*> &reduceTokens)
{
    // 1. Pop states from parsing stack until a state is found with nonempty Goto entries
    // 2. If there is a legal action on the current input token from one of the Goto states,
    //    push that state into the stack and restart the parse. If there are several such states,
    //    prefer a shift to a reduce. Among the reduce actions, prefer one whose associated nonterminal is least general
    // 3. If there is no legal action on the current input token from one of the Goto states, advance the input until there is a legal action
    //    or the end of the input is reached.

    ParsingTable *table=getParsingTable();

    while(!stack.isEmpty()){
        int stateOnTop = stack.pop();
        reduceTokens.append(tokenStack.pop());

        ParsingTableRow * row = table->rows.at(stateOnTop);
        if(!row->gotos.isEmpty()){ //step 1 done.
            //step 2. pick up a state that has an action on current token
            int gotoState = findStateWithAtionOnToken(*token, row, table);
            if(gotoState){
                stack.push(gotoState);
                tokenStack.push(createTokenInfo(*token));
                return true;
            }else{ //step 3.
                do{
                    *token = scanner->getNextToken();

                    reduceTokens.prepend(createTokenInfo(*token));

                    if(row->actions->value(*token, 0)){
                        return true;
                    }
                }while(*token!=PLS_SEMI && *token!=PLS_E_O_F);
            }
        }
    }

    return false;
}

int PlSqlParser::findStateWithAtionOnToken(int token, ParsingTableRow * row, ParsingTable *table) const
{
    ParsingTableAction *resultAction = 0;
    int resultState = 0;

    QHashIterator<int,int> i(row->gotos);
    while (i.hasNext()) {
        i.next();

        int targetState = i.value();
        ParsingTableRow *targetRow = table->rows.at(targetState);
        ParsingTableAction *targetAction = targetRow->actions->value(token, 0);

        if(!targetAction){
            continue;
        }

        if(!resultAction){
            resultAction = targetAction;
            resultState = targetState;
            continue;
        }

        if(targetAction->actionType == ParsingTableAction::Shift &&
                resultAction->actionType == ParsingTableAction::Reduce){
            resultAction = targetAction;
            resultState = targetState;
            continue;
        }

        if(targetAction->actionType == ParsingTableAction::Reduce &&
                resultAction->actionType == ParsingTableAction::Reduce &&
                targetAction->stateOrRuleId > resultAction->stateOrRuleId){ //rules with higher values should be more specific (least general) in grammar file
            resultAction = targetAction;
            resultState = targetState;
            continue;
        }
    }

    return resultState;
}*/
