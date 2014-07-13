#include "plsqlparsingtable.h"

PlSqlParsingTable *PlSqlParsingTable::instance = 0;

{action_declarations}

PlSqlParsingTable::PlSqlParsingTable()
{
}

PlSqlParsingTable *PlSqlParsingTable::getInstance()
{
    if(PlSqlParsingTable::instance==0){
        qDebug("initializing instance of parsing table for plsql");

        PlSqlParsingTable::instance = new PlSqlParsingTable();
        PlSqlParsingTable::instance->populateTable();
    }

    return PlSqlParsingTable::instance;
}

void PlSqlParsingTable::populateTable()
{
    populateTokenNames();
    populateKeywords();
    populateReservedWords();
    populateRuleNames();
    populateRuleOptions();
    initializeActions();
    createStates();
}

void PlSqlParsingTable::populateTokenNames()
{
    tokenNames["PLS_QUOTED_STRING"] = PLS_QUOTED_STRING;
    tokenNames["PLS_ID"] = PLS_ID;
    tokenNames["PLS_SEMI"] = PLS_SEMI;
    tokenNames["PLS_COLON"] = PLS_COLON;
    tokenNames["PLS_DOT"] = PLS_DOT;
    tokenNames["PLS_DOUBLE_DOT"] = PLS_DOUBLE_DOT;
    tokenNames["PLS_COMMA"] = PLS_COMMA;
    tokenNames["PLS_ASTERISK"] = PLS_ASTERISK;
    tokenNames["PLS_EXPONENT"] = PLS_EXPONENT;
    tokenNames["PLS_AT_SIGN"] = PLS_AT_SIGN;
    tokenNames["PLS_LPAREN"] = PLS_LPAREN;
    tokenNames["PLS_RPAREN"] = PLS_RPAREN;
    tokenNames["PLS_LBRACK"] = PLS_LBRACK;
    tokenNames["PLS_RBRACK"] = PLS_RBRACK;
    tokenNames["PLS_LCURLY_BRACK"] = PLS_LCURLY_BRACK;
    tokenNames["PLS_RCURLY_BRACK"] = PLS_RCURLY_BRACK;
    tokenNames["PLS_PLUS"] = PLS_PLUS;
    tokenNames["PLS_MINUS"] = PLS_MINUS;
    tokenNames["PLS_DIVIDE"] = PLS_DIVIDE;
    tokenNames["PLS_EQ"] = PLS_EQ;
    tokenNames["PLS_PERCENTAGE"] = PLS_PERCENTAGE;
    tokenNames["PLS_LLABEL"] = PLS_LLABEL;
    tokenNames["PLS_RLABEL"] = PLS_RLABEL;
    tokenNames["PLS_ASSIGN"] = PLS_ASSIGN;
    tokenNames["PLS_ARROW"] = PLS_ARROW;
    tokenNames["PLS_VERTBAR"] = PLS_VERTBAR;
    tokenNames["PLS_DOUBLEVERTBAR"] = PLS_DOUBLEVERTBAR;
    tokenNames["PLS_NOT_EQ"] = PLS_NOT_EQ;
    tokenNames["PLS_LESS_THAN"] = PLS_LESS_THAN;
    tokenNames["PLS_LESS_THAN_EQUAL"] = PLS_LESS_THAN_EQUAL;
    tokenNames["PLS_GREATER_THAN"] = PLS_GREATER_THAN;
    tokenNames["PLS_GREATER_THAN_EQUAL"] = PLS_GREATER_THAN_EQUAL;
    tokenNames["PLS_NUMBER"] = PLS_NUMBER;
    tokenNames["PLS_DOUBLEQUOTED_STRING"] = PLS_DOUBLEQUOTED_STRING;
    tokenNames["PLS_SL_COMMENT"] = PLS_SL_COMMENT;
    tokenNames["PLS_ML_COMMENT"] = PLS_ML_COMMENT;
    tokenNames["PLS_QUESTION_MARK"] = PLS_QUESTION_MARK;
    tokenNames["PLS_E_O_F"] = PLS_E_O_F;
    tokenNames["PLS_ERROR"] = PLS_ERROR;
    tokenNames["PLS_NOT_SEMI"] = PLS_NOT_SEMI;
}

void PlSqlParsingTable::populateKeywords()
{
   {keyword_initializations}
}

void PlSqlParsingTable::populateReservedWords()
{
   {reserved_word_initializations}
}

void PlSqlParsingTable::populateRuleNames()
{
   {rule_name_initializations}
}

void PlSqlParsingTable::populateRuleOptions()
{
   {rule_option_initializations}
}

void PlSqlParsingTable::initializeActions()
{
   {action_initializations}
}

void PlSqlParsingTable::createStates()
{
   {state_initializations}
}
