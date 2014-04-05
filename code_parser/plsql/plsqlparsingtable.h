#ifndef PLSQLPARSINGTABLE_H
#define PLSQLPARSINGTABLE_H

#include "../parsingtable.h"
#include "plsqltokens.h"

class PlSqlParsingTable : public ParsingTable
{
public:
    static ParsingTable *getInstance();

protected:
    PlSqlParsingTable();

private:
    void populateTable();
    void populateKeywords();
    void populateRuleNames();
    void initializeActions();
    void createStates();

    static PlSqlParsingTable *instance;
};

#endif // PLSQLPARSINGTABLE_H
