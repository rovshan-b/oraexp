#include "plsqlparsingtable.h"

PlSqlParsingTable *PlSqlParsingTable::instance = 0;

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
    //---------target scanner keywords------------
keywords.reserve(4);
keywords.append("CREATE");
keywords.append("OR");
keywords.append("PACKAGE");
keywords.append("REPLACE");
//-------end target scanner keywords----------
//-----------rule string representations-----------------
ruleNames.reserve( 5 );
ruleNames.append("CREATE_PACKAGE_AUG");
ruleNames.append("CREATE_PACKAGE");
ruleNames.append("OPT_OR_REPLACE");
ruleNames.append("OR_REPLACE");
ruleNames.append("PACKAGE");
//-------------------------------------------------------
//----------------parsing table code for target parser---------------
ParsingTableAction * action_0_0 = new ParsingTableAction();
action_0_0->actionType = ParsingTableAction::Shift;
action_0_0->stateOrRuleId = 2;
action_0_0->symbolCount = -1;

ParsingTableAction * action_1_PLS_E_O_F = new ParsingTableAction();
action_1_PLS_E_O_F->actionType = ParsingTableAction::Accept;
action_1_PLS_E_O_F->stateOrRuleId = -1;
action_1_PLS_E_O_F->symbolCount = -1;

ParsingTableAction * action_2_1 = new ParsingTableAction();
action_2_1->actionType = ParsingTableAction::Shift;
action_2_1->stateOrRuleId = 5;
action_2_1->symbolCount = -1;

ParsingTableAction * action_2_2 = new ParsingTableAction();
action_2_2->actionType = ParsingTableAction::Reduce;
action_2_2->stateOrRuleId = 3;
action_2_2->symbolCount = 0;

ParsingTableAction * action_3_2 = new ParsingTableAction();
action_3_2->actionType = ParsingTableAction::Shift;
action_3_2->stateOrRuleId = 7;
action_3_2->symbolCount = -1;

ParsingTableAction * action_4_2 = new ParsingTableAction();
action_4_2->actionType = ParsingTableAction::Reduce;
action_4_2->stateOrRuleId = 3;
action_4_2->symbolCount = 1;

ParsingTableAction * action_5_3 = new ParsingTableAction();
action_5_3->actionType = ParsingTableAction::Shift;
action_5_3->stateOrRuleId = 8;
action_5_3->symbolCount = -1;

ParsingTableAction * action_6_PLS_E_O_F = new ParsingTableAction();
action_6_PLS_E_O_F->actionType = ParsingTableAction::Reduce;
action_6_PLS_E_O_F->stateOrRuleId = 2;
action_6_PLS_E_O_F->symbolCount = 3;

ParsingTableAction * action_7_PLS_E_O_F = new ParsingTableAction();
action_7_PLS_E_O_F->actionType = ParsingTableAction::Reduce;
action_7_PLS_E_O_F->stateOrRuleId = 5;
action_7_PLS_E_O_F->symbolCount = 1;

ParsingTableAction * action_8_2 = new ParsingTableAction();
action_8_2->actionType = ParsingTableAction::Reduce;
action_8_2->stateOrRuleId = 4;
action_8_2->symbolCount = 2;

rows.reserve( 9 );

ParsingTableRow *row_0 = new ParsingTableRow();
row_0->stateId = 0;
row_0->ownsActions = true;
(*row_0->actions)[0] = action_0_0;
row_0->gotos[2] = 1;
rows.append(row_0);

ParsingTableRow *row_1 = new ParsingTableRow();
row_1->stateId = 1;
row_1->ownsActions = true;
(*row_1->actions)[PLS_E_O_F] = action_1_PLS_E_O_F;
rows.append(row_1);

ParsingTableRow *row_2 = new ParsingTableRow();
row_2->stateId = 2;
row_2->ownsActions = true;
(*row_2->actions)[1] = action_2_1;
(*row_2->actions)[2] = action_2_2;
row_2->gotos[3] = 3;
row_2->gotos[4] = 4;
rows.append(row_2);

ParsingTableRow *row_3 = new ParsingTableRow();
row_3->stateId = 3;
row_3->ownsActions = true;
(*row_3->actions)[2] = action_3_2;
row_3->gotos[5] = 6;
rows.append(row_3);

ParsingTableRow *row_4 = new ParsingTableRow();
row_4->stateId = 4;
row_4->ownsActions = true;
(*row_4->actions)[2] = action_4_2;
rows.append(row_4);

ParsingTableRow *row_5 = new ParsingTableRow();
row_5->stateId = 5;
row_5->ownsActions = true;
(*row_5->actions)[3] = action_5_3;
rows.append(row_5);

ParsingTableRow *row_6 = new ParsingTableRow();
row_6->stateId = 6;
row_6->ownsActions = true;
(*row_6->actions)[PLS_E_O_F] = action_6_PLS_E_O_F;
rows.append(row_6);

ParsingTableRow *row_7 = new ParsingTableRow();
row_7->stateId = 7;
row_7->ownsActions = true;
(*row_7->actions)[PLS_E_O_F] = action_7_PLS_E_O_F;
rows.append(row_7);

ParsingTableRow *row_8 = new ParsingTableRow();
row_8->stateId = 8;
row_8->ownsActions = true;
(*row_8->actions)[2] = action_8_2;
rows.append(row_8);

//--------------end parsing table code for target parser-------------

}
