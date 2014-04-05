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
    if(*token!=PLS_SEMI){
        *actionOnCurrentToken=row->actions->value(PLS_NOT_SEMI, 0);

        replaceKeywordWithIdentifier(*token, row, actionOnCurrentToken);
    }

    if(*actionOnCurrentToken == 0){ //try to recognize major constructs
        //read input until we encounter one of (first of) PLS_SEMI, END opt_identifier PLS_SEMI
        //while reading add all read tokens to token stack
        QList<TokenInfo*> reduceTokens;
        do{
            reduceTokens.prepend(createTokenInfo(*token));

            if(*token == PLS_SEMI){
                if(reduceMajorConstruct(reduceTokens)){
                    //read next token for parser to operate on
                    *token = scanner->getNextToken();
                    ParsingTableRow *newRow=parsingTable->rows.at(stack.top()); //reduceMajorConstruct pushed new gotoState into stack
                    *actionOnCurrentToken=(*newRow->actions).value(*token, 0);

                    replaceKeywordWithIdentifier(*token, newRow, actionOnCurrentToken);
                }else{
                    qDeleteAll(reduceTokens);
                }
                break;
            }

            *token = scanner->getNextToken();
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
    const int majorConstructCount = 2;
    const int majorConstructs[] = {R_DECLARATION, R_STATEMENT};

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
