#include "plsqlparsingtable.h"

PlSqlParsingTable *PlSqlParsingTable::instance = 0;

{action_declarations}

PlSqlParsingTable::PlSqlParsingTable()
{
}

ParsingTable *PlSqlParsingTable::getInstance()
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
    populateKeywords();
    populateReservedWords();
    populateRuleNames();
    populateRuleOptions();
    initializeActions();
    createStates();
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
