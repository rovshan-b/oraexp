#include "codeparser.h"
#include "ireducelistener.h"
#include "beans/tokeninfo.h"
#include "plsql/plsqltokens.h"

CodeParser::CodeParser(CodeScanner *scanner) : scanner(scanner), errorRow(0), reduceListener(0)
{
}

CodeParser::~CodeParser()
{
    delete scanner;
}

bool CodeParser::parse()
{
    Q_ASSERT(scanner);

    ParsingTable *table=getParsingTable();
    Q_ASSERT(table);

    //stack.clear();
    QStack<int> stack;
    stack.push(0);

    QStack<TokenInfo*> tokenStack;
    tokenStack.push(0);

    QList<TokenInfo*> currentReduceTokens;

    int token = scanner->getNextToken();

    ParsingTableRow *row;
    ParsingTableAction *actionOnCurrToken;
    ParsingTableAction::ActionType actionType;

    int ruleId;
    int gotoState;

    bool done=false;

    while(true){
        int stateOnTop = stack.top();

        row=table->rows.at(stateOnTop);

        actionOnCurrToken=(*row->actions).value(token, 0);

        if(actionOnCurrToken==0){
            actionOnCurrToken=(*row->actions).value(PLS_ANY, 0);
        }

        if(actionOnCurrToken==0){ //parsing error
            //qDebug("parsing error");
            errorRow = row;
            if(reduceListener){
                reduceListener->error();
            }
            return false;
            break;
        }

        actionType=actionOnCurrToken->actionType;

        switch(actionType){
        case ParsingTableAction::Shift:
        {
            stack.push(actionOnCurrToken->stateOrRuleId);

            TokenInfo *ti = new TokenInfo();
            ti->tokenType = TokenInfo::Token;
            ti->tokenOrRuleId = token;
            ti->startPos = scanner->getTokenStartPos();
            ti->endPos = scanner->getTokenEndPos();
            ti->startLine = scanner->getTokenStartLine();
            ti->endLine = scanner->getTokenEndLine();
            ti->startLinePos = scanner->getTokenStartLinePos();
            ti->endLinePos = scanner->getTokenEndLinePos();
            ti->lexeme = scanner->getTokenLexeme();
            tokenStack.push(ti);

            token=scanner->getNextToken();
            break;
        }
        case ParsingTableAction::Reduce:
        {
            ruleId=actionOnCurrToken->stateOrRuleId;
            currentReduceTokens.clear();
            for(int i=0; i<actionOnCurrToken->symbolCount; ++i){
                stack.pop();
                currentReduceTokens.append(tokenStack.pop());
            }
            stateOnTop = stack.top();
            row=table->rows.at(stateOnTop);
            gotoState=row->gotos.value(ruleId, -1);
            Q_ASSERT(gotoState!=-1);
            stack.push(gotoState);
            TokenInfo *ti = new TokenInfo();
            ti->tokenType = TokenInfo::Rule;
            ti->tokenOrRuleId = ruleId;
            tokenStack.push(ti);
            if(this->reduceListener){
                this->reduceListener->reduced(ti, actionOnCurrToken->symbolCount, currentReduceTokens, table);
            }
            break;
        }
        case ParsingTableAction::Accept:
            done=true;
            if(this->reduceListener){
                this->reduceListener->accepted();
            }
            break;
        }

        if(done){
            break;
        }
    }

    errorRow=0;

    return true;
}

QPair< QList<int>, QList<int> > CodeParser::getExpectedTokens() const
{
    QList<int> terminals;
    QList<int> rules;

    if(errorRow!=0){
        QHash<int, ParsingTableAction*>::const_iterator i1 = errorRow->actions->constBegin();
        while (i1 != errorRow->actions->constEnd()) {
            terminals.append(i1.key());

            ++i1;
        }

        QHash<int, int>::const_iterator i2 = errorRow->gotos.constBegin();
        while (i2 != errorRow->gotos.constEnd()) {
            rules.append(i2.key());

            ++i2;
        }
    }

    return qMakePair(terminals, rules);
}
