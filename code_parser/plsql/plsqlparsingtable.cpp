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
keywords.reserve(5);
keywords.append("CREATE");
keywords.append("NAME");
keywords.append("OR");
keywords.append("PACKAGE");
keywords.append("REPLACE");
//-------end target scanner keywords----------
//-----------rule string representations-----------------
ruleNames.reserve( 5 );
ruleNames.append("CREATE_PACKAGE_AUG");
ruleNames.append("CREATE_PACKAGE");
ruleNames.append("OR_REPLACE");
ruleNames.append("PACKAGE");
ruleNames.append("NAME");
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

ParsingTableAction * action_2_2 = new ParsingTableAction();
action_2_2->actionType = ParsingTableAction::Shift;
action_2_2->stateOrRuleId = 4;
action_2_2->symbolCount = -1;

ParsingTableAction * action_3_1 = new ParsingTableAction();
action_3_1->actionType = ParsingTableAction::Reduce;
action_3_1->stateOrRuleId = 4;
action_3_1->symbolCount = 0;

ParsingTableAction * action_3_3 = new ParsingTableAction();
action_3_3->actionType = ParsingTableAction::Shift;
action_3_3->stateOrRuleId = 6;
action_3_3->symbolCount = -1;

ParsingTableAction * action_4_4 = new ParsingTableAction();
action_4_4->actionType = ParsingTableAction::Shift;
action_4_4->stateOrRuleId = 7;
action_4_4->symbolCount = -1;

ParsingTableAction * action_5_1 = new ParsingTableAction();
action_5_1->actionType = ParsingTableAction::Shift;
action_5_1->stateOrRuleId = 9;
action_5_1->symbolCount = -1;

ParsingTableAction * action_6_1 = new ParsingTableAction();
action_6_1->actionType = ParsingTableAction::Reduce;
action_6_1->stateOrRuleId = 4;
action_6_1->symbolCount = 1;

ParsingTableAction * action_7_1 = new ParsingTableAction();
action_7_1->actionType = ParsingTableAction::Reduce;
action_7_1->stateOrRuleId = 3;
action_7_1->symbolCount = 2;

ParsingTableAction * action_7_3 = new ParsingTableAction();
action_7_3->actionType = ParsingTableAction::Reduce;
action_7_3->stateOrRuleId = 3;
action_7_3->symbolCount = 2;

ParsingTableAction * action_8_PLS_E_O_F = new ParsingTableAction();
action_8_PLS_E_O_F->actionType = ParsingTableAction::Reduce;
action_8_PLS_E_O_F->stateOrRuleId = 2;
action_8_PLS_E_O_F->symbolCount = 4;

ParsingTableAction * action_9_PLS_E_O_F = new ParsingTableAction();
action_9_PLS_E_O_F->actionType = ParsingTableAction::Reduce;
action_9_PLS_E_O_F->stateOrRuleId = 5;
action_9_PLS_E_O_F->symbolCount = 1;

rows.reserve( 10 );

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
(*row_2->actions)[2] = action_2_2;
row_2->gotos[3] = 3;
rows.append(row_2);

ParsingTableRow *row_3 = new ParsingTableRow();
row_3->stateId = 3;
row_3->ownsActions = true;
(*row_3->actions)[1] = action_3_1;
(*row_3->actions)[3] = action_3_3;
row_3->gotos[4] = 5;
rows.append(row_3);

ParsingTableRow *row_4 = new ParsingTableRow();
row_4->stateId = 4;
row_4->ownsActions = true;
(*row_4->actions)[4] = action_4_4;
rows.append(row_4);

ParsingTableRow *row_5 = new ParsingTableRow();
row_5->stateId = 5;
row_5->ownsActions = true;
(*row_5->actions)[1] = action_5_1;
row_5->gotos[5] = 8;
rows.append(row_5);

ParsingTableRow *row_6 = new ParsingTableRow();
row_6->stateId = 6;
row_6->ownsActions = true;
(*row_6->actions)[1] = action_6_1;
rows.append(row_6);

ParsingTableRow *row_7 = new ParsingTableRow();
row_7->stateId = 7;
row_7->ownsActions = true;
(*row_7->actions)[1] = action_7_1;
(*row_7->actions)[3] = action_7_3;
rows.append(row_7);

ParsingTableRow *row_8 = new ParsingTableRow();
row_8->stateId = 8;
row_8->ownsActions = true;
(*row_8->actions)[PLS_E_O_F] = action_8_PLS_E_O_F;
rows.append(row_8);

ParsingTableRow *row_9 = new ParsingTableRow();
row_9->stateId = 9;
row_9->ownsActions = true;
(*row_9->actions)[PLS_E_O_F] = action_9_PLS_E_O_F;
rows.append(row_9);

//--------------end parsing table code for target parser-------------

}
