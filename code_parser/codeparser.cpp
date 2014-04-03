#include "codeparser.h"
#include "ireducelistener.h"
#include "beans/tokeninfo.h"
#include "plsql/plsqltokens.h"

#include <QDebug>

CodeParser::CodeParser(CodeScanner *scanner) : scanner(scanner), parsingTable(0), errorRow(0), reduceListener(0)
{
}

CodeParser::~CodeParser()
{
    delete scanner;
}

bool CodeParser::parse()
{
    Q_ASSERT(scanner);

    stack.clear();
    stack.push(0);

    tokenStack.clear();
    tokenStack.push(0);

    int token = scanner->getNextToken();

    if(parsingTable == 0){
        parsingTable=getParsingTable();
    }
    Q_ASSERT(parsingTable);
    ParsingTableRow *row;
    ParsingTableAction *actionOnCurrToken;
    ParsingTableAction::ActionType actionType;

    bool done=false;

    while(true){
        int stateOnTop = stack.top();

        //qDebug() << "IN STATE " << stateOnTop << " TOKEN " << token << "  " << scanner->getTokenLexeme();

        row=parsingTable->rows.at(stateOnTop);

        actionOnCurrToken=(*row->actions).value(token, 0);

        if(actionOnCurrToken==0){
            correctError(&token, row, &actionOnCurrToken);
        }

        if(actionOnCurrToken==0){ //parsing error

            qDebug() << "parse failed in state" << stateOnTop << ", current token is" << token << " (lexeme =" << scanner->getTokenLexeme() << ")" ;

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
            shift(actionOnCurrToken->stateOrRuleId, &token);
            break;
        }
        case ParsingTableAction::Reduce:
        {
            reduce(actionOnCurrToken->stateOrRuleId, actionOnCurrToken->symbolCount);
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

void CodeParser::shift(int stateId, int *token)
{
    stack.push(stateId);
    tokenStack.push(createTokenInfo(*token));

    //qDebug() << "shifting to state " << stateId;

    *token=scanner->getNextToken();
}

void CodeParser::reduce(int ruleId, int symbolCount, const QList<TokenInfo *> &additionalReduceTokens)
{
    //qDebug() << "reducing by rule " << parsingTable->getRuleName(ruleId);

    QList<TokenInfo*> currentReduceTokens(additionalReduceTokens);
    for(int i=0; i<symbolCount; ++i){
        stack.pop();
        currentReduceTokens.append(tokenStack.pop());
    }
    int stateOnTop = stack.top();
    ParsingTableRow *row=parsingTable->rows.at(stateOnTop);
    int gotoState=row->gotos.value(ruleId, -1);
    Q_ASSERT(gotoState!=-1);
    stack.push(gotoState);
    TokenInfo *ti = new TokenInfo();
    ti->tokenType = TokenInfo::Rule;
    ti->tokenOrRuleId = ruleId;
    tokenStack.push(ti);
    if(this->reduceListener){
        this->reduceListener->reduced(ti, symbolCount, currentReduceTokens, parsingTable);
    }

    //qDebug() << "going to state " << gotoState;
}


TokenInfo *CodeParser::createTokenInfo(int token) const
{
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

    return ti;
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
