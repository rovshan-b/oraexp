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
keywords.reserve(78);
keywords.append("AND");
keywords.append("ANY");
keywords.append("ASC");
keywords.append("BETWEEN");
keywords.append("BODY");
keywords.append("CHAR");
keywords.append("CHECK");
keywords.append("CLUSTER");
keywords.append("COMPRESS");
keywords.append("CONNECT");
keywords.append("CREATE");
keywords.append("DATE");
keywords.append("DECIMAL");
keywords.append("DEFAULT");
keywords.append("DELETE");
keywords.append("DESC");
keywords.append("DISTINCT");
keywords.append("DROP");
keywords.append("ELSE");
keywords.append("EXCLUSIVE");
keywords.append("EXISTS");
keywords.append("FLOAT");
keywords.append("FOR");
keywords.append("FORCE");
keywords.append("FROM");
keywords.append("FUNCTION");
keywords.append("GRANT");
keywords.append("GROUP");
keywords.append("HAVING");
keywords.append("IDENTIFIED");
keywords.append("INDEX");
keywords.append("INSERT");
keywords.append("INTEGER");
keywords.append("INTERSECT");
keywords.append("INTO");
keywords.append("LIKE");
keywords.append("LOCK");
keywords.append("LONG");
keywords.append("MINUS");
keywords.append("MODE");
keywords.append("NO");
keywords.append("NOCOMPRESS");
keywords.append("NOT");
keywords.append("NOWAIT");
keywords.append("NULL");
keywords.append("NUMBER");
keywords.append("OPTION");
keywords.append("OR");
keywords.append("ORDER");
keywords.append("PACKAGE");
keywords.append("PCTFREE");
keywords.append("PRIOR");
keywords.append("PROCEDURE");
keywords.append("PUBLIC");
keywords.append("RAW");
keywords.append("RENAME");
keywords.append("REPLACE");
keywords.append("RESOURCE");
keywords.append("REVOKE");
keywords.append("SELECT");
keywords.append("SET");
keywords.append("SHARE");
keywords.append("SIZE");
keywords.append("SMALLINT");
keywords.append("START");
keywords.append("SYNONYM");
keywords.append("TABLE");
keywords.append("THEN");
keywords.append("TRIGGER");
keywords.append("UNION");
keywords.append("UNIQUE");
keywords.append("UPDATE");
keywords.append("VALUES");
keywords.append("VARCHAR");
keywords.append("VARCHAR2");
keywords.append("VIEW");
keywords.append("WHERE");
keywords.append("WITH");
//-------end target scanner keywords----------
//-----------rule string representations-----------------
ruleNames.reserve( 13 );
ruleNames.append("START_RULE_AUG");
ruleNames.append("START_RULE");
ruleNames.append("CREATE_OBJECT");
ruleNames.append("OPT_OR_REPLACE");
ruleNames.append("OBJ_TYPE_AND_NAME");
ruleNames.append("OBJ_SPEC_OR_BODY");
ruleNames.append("ANY_TOKENS");
ruleNames.append("OPT_FORCE");
ruleNames.append("OPT_NO");
ruleNames.append("NO");
ruleNames.append("FORCE");
ruleNames.append("OBJECT_NAME");
ruleNames.append("IDENTIFIER");
//-------------------------------------------------------
//----------------parsing table code for target parser---------------
ParsingTableAction * action_0_10 = new ParsingTableAction();
action_0_10->actionType = ParsingTableAction::Shift;
action_0_10->stateOrRuleId = 3;
action_0_10->symbolCount = -1;

ParsingTableAction * action_1_PLS_E_O_F = new ParsingTableAction();
action_1_PLS_E_O_F->actionType = ParsingTableAction::Accept;
action_1_PLS_E_O_F->stateOrRuleId = -1;
action_1_PLS_E_O_F->symbolCount = -1;

ParsingTableAction * action_2_PLS_E_O_F = new ParsingTableAction();
action_2_PLS_E_O_F->actionType = ParsingTableAction::Reduce;
action_2_PLS_E_O_F->stateOrRuleId = 2;
action_2_PLS_E_O_F->symbolCount = 1;

ParsingTableAction * action_3_68 = new ParsingTableAction();
action_3_68->actionType = ParsingTableAction::Reduce;
action_3_68->stateOrRuleId = 4;
action_3_68->symbolCount = 0;

ParsingTableAction * action_3_52 = new ParsingTableAction();
action_3_52->actionType = ParsingTableAction::Reduce;
action_3_52->stateOrRuleId = 4;
action_3_52->symbolCount = 0;

ParsingTableAction * action_3_23 = new ParsingTableAction();
action_3_23->actionType = ParsingTableAction::Reduce;
action_3_23->stateOrRuleId = 4;
action_3_23->symbolCount = 0;

ParsingTableAction * action_3_40 = new ParsingTableAction();
action_3_40->actionType = ParsingTableAction::Reduce;
action_3_40->stateOrRuleId = 4;
action_3_40->symbolCount = 0;

ParsingTableAction * action_3_75 = new ParsingTableAction();
action_3_75->actionType = ParsingTableAction::Reduce;
action_3_75->stateOrRuleId = 4;
action_3_75->symbolCount = 0;

ParsingTableAction * action_3_25 = new ParsingTableAction();
action_3_25->actionType = ParsingTableAction::Reduce;
action_3_25->stateOrRuleId = 4;
action_3_25->symbolCount = 0;

ParsingTableAction * action_3_47 = new ParsingTableAction();
action_3_47->actionType = ParsingTableAction::Shift;
action_3_47->stateOrRuleId = 5;
action_3_47->symbolCount = -1;

ParsingTableAction * action_3_49 = new ParsingTableAction();
action_3_49->actionType = ParsingTableAction::Reduce;
action_3_49->stateOrRuleId = 4;
action_3_49->symbolCount = 0;

ParsingTableAction * action_4_68 = new ParsingTableAction();
action_4_68->actionType = ParsingTableAction::Shift;
action_4_68->stateOrRuleId = 10;
action_4_68->symbolCount = -1;

ParsingTableAction * action_4_52 = new ParsingTableAction();
action_4_52->actionType = ParsingTableAction::Shift;
action_4_52->stateOrRuleId = 8;
action_4_52->symbolCount = -1;

ParsingTableAction * action_4_23 = new ParsingTableAction();
action_4_23->actionType = ParsingTableAction::Shift;
action_4_23->stateOrRuleId = 15;
action_4_23->symbolCount = -1;

ParsingTableAction * action_4_40 = new ParsingTableAction();
action_4_40->actionType = ParsingTableAction::Shift;
action_4_40->stateOrRuleId = 16;
action_4_40->symbolCount = -1;

ParsingTableAction * action_4_75 = new ParsingTableAction();
action_4_75->actionType = ParsingTableAction::Reduce;
action_4_75->stateOrRuleId = 8;
action_4_75->symbolCount = 0;

ParsingTableAction * action_4_25 = new ParsingTableAction();
action_4_25->actionType = ParsingTableAction::Shift;
action_4_25->stateOrRuleId = 9;
action_4_25->symbolCount = -1;

ParsingTableAction * action_4_49 = new ParsingTableAction();
action_4_49->actionType = ParsingTableAction::Shift;
action_4_49->stateOrRuleId = 7;
action_4_49->symbolCount = -1;

ParsingTableAction * action_5_56 = new ParsingTableAction();
action_5_56->actionType = ParsingTableAction::Shift;
action_5_56->stateOrRuleId = 17;
action_5_56->symbolCount = -1;

ParsingTableAction * action_6_PLS_ANY = new ParsingTableAction();
action_6_PLS_ANY->actionType = ParsingTableAction::Shift;
action_6_PLS_ANY->stateOrRuleId = 20;
action_6_PLS_ANY->symbolCount = -1;

ParsingTableAction * action_6_PLS_E_O_F = new ParsingTableAction();
action_6_PLS_E_O_F->actionType = ParsingTableAction::Reduce;
action_6_PLS_E_O_F->stateOrRuleId = 7;
action_6_PLS_E_O_F->symbolCount = 0;

ParsingTableAction * action_7_4 = new ParsingTableAction();
action_7_4->actionType = ParsingTableAction::Shift;
action_7_4->stateOrRuleId = 22;
action_7_4->symbolCount = -1;

ParsingTableAction * action_7_PLS_ID = new ParsingTableAction();
action_7_PLS_ID->actionType = ParsingTableAction::Shift;
action_7_PLS_ID->stateOrRuleId = 24;
action_7_PLS_ID->symbolCount = -1;

ParsingTableAction * action_7_PLS_DOUBLEQUOTED_STRING = new ParsingTableAction();
action_7_PLS_DOUBLEQUOTED_STRING->actionType = ParsingTableAction::Shift;
action_7_PLS_DOUBLEQUOTED_STRING->stateOrRuleId = 25;
action_7_PLS_DOUBLEQUOTED_STRING->symbolCount = -1;

ParsingTableAction * action_8_PLS_ID = new ParsingTableAction();
action_8_PLS_ID->actionType = ParsingTableAction::Shift;
action_8_PLS_ID->stateOrRuleId = 24;
action_8_PLS_ID->symbolCount = -1;

ParsingTableAction * action_8_PLS_DOUBLEQUOTED_STRING = new ParsingTableAction();
action_8_PLS_DOUBLEQUOTED_STRING->actionType = ParsingTableAction::Shift;
action_8_PLS_DOUBLEQUOTED_STRING->stateOrRuleId = 25;
action_8_PLS_DOUBLEQUOTED_STRING->symbolCount = -1;

ParsingTableAction * action_11_75 = new ParsingTableAction();
action_11_75->actionType = ParsingTableAction::Shift;
action_11_75->stateOrRuleId = 29;
action_11_75->symbolCount = -1;

ParsingTableAction * action_12_23 = new ParsingTableAction();
action_12_23->actionType = ParsingTableAction::Shift;
action_12_23->stateOrRuleId = 15;
action_12_23->symbolCount = -1;

ParsingTableAction * action_13_75 = new ParsingTableAction();
action_13_75->actionType = ParsingTableAction::Reduce;
action_13_75->stateOrRuleId = 8;
action_13_75->symbolCount = 1;

ParsingTableAction * action_14_23 = new ParsingTableAction();
action_14_23->actionType = ParsingTableAction::Reduce;
action_14_23->stateOrRuleId = 9;
action_14_23->symbolCount = 1;

ParsingTableAction * action_15_75 = new ParsingTableAction();
action_15_75->actionType = ParsingTableAction::Reduce;
action_15_75->stateOrRuleId = 11;
action_15_75->symbolCount = 1;

ParsingTableAction * action_16_23 = new ParsingTableAction();
action_16_23->actionType = ParsingTableAction::Reduce;
action_16_23->stateOrRuleId = 10;
action_16_23->symbolCount = 1;

ParsingTableAction * action_17_68 = new ParsingTableAction();
action_17_68->actionType = ParsingTableAction::Reduce;
action_17_68->stateOrRuleId = 4;
action_17_68->symbolCount = 2;

ParsingTableAction * action_17_52 = new ParsingTableAction();
action_17_52->actionType = ParsingTableAction::Reduce;
action_17_52->stateOrRuleId = 4;
action_17_52->symbolCount = 2;

ParsingTableAction * action_17_23 = new ParsingTableAction();
action_17_23->actionType = ParsingTableAction::Reduce;
action_17_23->stateOrRuleId = 4;
action_17_23->symbolCount = 2;

ParsingTableAction * action_17_40 = new ParsingTableAction();
action_17_40->actionType = ParsingTableAction::Reduce;
action_17_40->stateOrRuleId = 4;
action_17_40->symbolCount = 2;

ParsingTableAction * action_17_75 = new ParsingTableAction();
action_17_75->actionType = ParsingTableAction::Reduce;
action_17_75->stateOrRuleId = 4;
action_17_75->symbolCount = 2;

ParsingTableAction * action_17_25 = new ParsingTableAction();
action_17_25->actionType = ParsingTableAction::Reduce;
action_17_25->stateOrRuleId = 4;
action_17_25->symbolCount = 2;

ParsingTableAction * action_17_49 = new ParsingTableAction();
action_17_49->actionType = ParsingTableAction::Reduce;
action_17_49->stateOrRuleId = 4;
action_17_49->symbolCount = 2;

ParsingTableAction * action_18_PLS_E_O_F = new ParsingTableAction();
action_18_PLS_E_O_F->actionType = ParsingTableAction::Reduce;
action_18_PLS_E_O_F->stateOrRuleId = 3;
action_18_PLS_E_O_F->symbolCount = 4;

ParsingTableAction * action_19_PLS_E_O_F = new ParsingTableAction();
action_19_PLS_E_O_F->actionType = ParsingTableAction::Reduce;
action_19_PLS_E_O_F->stateOrRuleId = 6;
action_19_PLS_E_O_F->symbolCount = 1;

ParsingTableAction * action_21_PLS_ANY = new ParsingTableAction();
action_21_PLS_ANY->actionType = ParsingTableAction::Reduce;
action_21_PLS_ANY->stateOrRuleId = 5;
action_21_PLS_ANY->symbolCount = 2;

ParsingTableAction * action_21_PLS_E_O_F = new ParsingTableAction();
action_21_PLS_E_O_F->actionType = ParsingTableAction::Reduce;
action_21_PLS_E_O_F->stateOrRuleId = 5;
action_21_PLS_E_O_F->symbolCount = 2;

ParsingTableAction * action_23_PLS_ANY = new ParsingTableAction();
action_23_PLS_ANY->actionType = ParsingTableAction::Reduce;
action_23_PLS_ANY->stateOrRuleId = 12;
action_23_PLS_ANY->symbolCount = 1;

ParsingTableAction * action_23_PLS_DOT = new ParsingTableAction();
action_23_PLS_DOT->actionType = ParsingTableAction::Shift;
action_23_PLS_DOT->stateOrRuleId = 33;
action_23_PLS_DOT->symbolCount = -1;

ParsingTableAction * action_23_PLS_E_O_F = new ParsingTableAction();
action_23_PLS_E_O_F->actionType = ParsingTableAction::Reduce;
action_23_PLS_E_O_F->stateOrRuleId = 12;
action_23_PLS_E_O_F->symbolCount = 1;

ParsingTableAction * action_24_PLS_ANY = new ParsingTableAction();
action_24_PLS_ANY->actionType = ParsingTableAction::Reduce;
action_24_PLS_ANY->stateOrRuleId = 13;
action_24_PLS_ANY->symbolCount = 1;

ParsingTableAction * action_24_PLS_DOT = new ParsingTableAction();
action_24_PLS_DOT->actionType = ParsingTableAction::Reduce;
action_24_PLS_DOT->stateOrRuleId = 13;
action_24_PLS_DOT->symbolCount = 1;

ParsingTableAction * action_24_PLS_E_O_F = new ParsingTableAction();
action_24_PLS_E_O_F->actionType = ParsingTableAction::Reduce;
action_24_PLS_E_O_F->stateOrRuleId = 13;
action_24_PLS_E_O_F->symbolCount = 1;

ParsingTableAction * action_30_75 = new ParsingTableAction();
action_30_75->actionType = ParsingTableAction::Reduce;
action_30_75->stateOrRuleId = 8;
action_30_75->symbolCount = 2;

ParsingTableAction * action_31_PLS_E_O_F = new ParsingTableAction();
action_31_PLS_E_O_F->actionType = ParsingTableAction::Reduce;
action_31_PLS_E_O_F->stateOrRuleId = 7;
action_31_PLS_E_O_F->symbolCount = 2;

ParsingTableAction * action_32_PLS_ANY = new ParsingTableAction();
action_32_PLS_ANY->actionType = ParsingTableAction::Reduce;
action_32_PLS_ANY->stateOrRuleId = 5;
action_32_PLS_ANY->symbolCount = 3;

ParsingTableAction * action_32_PLS_E_O_F = new ParsingTableAction();
action_32_PLS_E_O_F->actionType = ParsingTableAction::Reduce;
action_32_PLS_E_O_F->stateOrRuleId = 5;
action_32_PLS_E_O_F->symbolCount = 3;

ParsingTableAction * action_35_PLS_ANY = new ParsingTableAction();
action_35_PLS_ANY->actionType = ParsingTableAction::Reduce;
action_35_PLS_ANY->stateOrRuleId = 12;
action_35_PLS_ANY->symbolCount = 3;

ParsingTableAction * action_35_PLS_E_O_F = new ParsingTableAction();
action_35_PLS_E_O_F->actionType = ParsingTableAction::Reduce;
action_35_PLS_E_O_F->stateOrRuleId = 12;
action_35_PLS_E_O_F->symbolCount = 3;

rows.reserve( 36 );

ParsingTableRow *row_0 = new ParsingTableRow();
row_0->stateId = 0;
row_0->ownsActions = true;
(*row_0->actions)[10] = action_0_10;
row_0->gotos[2] = 1;
row_0->gotos[3] = 2;
rows.append(row_0);

ParsingTableRow *row_1 = new ParsingTableRow();
row_1->stateId = 1;
row_1->ownsActions = true;
(*row_1->actions)[PLS_E_O_F] = action_1_PLS_E_O_F;
rows.append(row_1);

ParsingTableRow *row_2 = new ParsingTableRow();
row_2->stateId = 2;
row_2->ownsActions = true;
(*row_2->actions)[PLS_E_O_F] = action_2_PLS_E_O_F;
rows.append(row_2);

ParsingTableRow *row_3 = new ParsingTableRow();
row_3->stateId = 3;
row_3->ownsActions = true;
(*row_3->actions)[68] = action_3_68;
(*row_3->actions)[52] = action_3_52;
(*row_3->actions)[23] = action_3_23;
(*row_3->actions)[40] = action_3_40;
(*row_3->actions)[75] = action_3_75;
(*row_3->actions)[25] = action_3_25;
(*row_3->actions)[47] = action_3_47;
(*row_3->actions)[49] = action_3_49;
row_3->gotos[4] = 4;
rows.append(row_3);

ParsingTableRow *row_4 = new ParsingTableRow();
row_4->stateId = 4;
row_4->ownsActions = true;
(*row_4->actions)[68] = action_4_68;
(*row_4->actions)[52] = action_4_52;
(*row_4->actions)[23] = action_4_23;
(*row_4->actions)[40] = action_4_40;
(*row_4->actions)[75] = action_4_75;
(*row_4->actions)[25] = action_4_25;
(*row_4->actions)[49] = action_4_49;
row_4->gotos[5] = 6;
row_4->gotos[8] = 11;
row_4->gotos[9] = 12;
row_4->gotos[10] = 14;
row_4->gotos[11] = 13;
rows.append(row_4);

ParsingTableRow *row_5 = new ParsingTableRow();
row_5->stateId = 5;
row_5->ownsActions = true;
(*row_5->actions)[56] = action_5_56;
rows.append(row_5);

ParsingTableRow *row_6 = new ParsingTableRow();
row_6->stateId = 6;
row_6->ownsActions = true;
(*row_6->actions)[PLS_ANY] = action_6_PLS_ANY;
(*row_6->actions)[PLS_E_O_F] = action_6_PLS_E_O_F;
row_6->gotos[6] = 18;
row_6->gotos[7] = 19;
rows.append(row_6);

ParsingTableRow *row_7 = new ParsingTableRow();
row_7->stateId = 7;
row_7->ownsActions = true;
(*row_7->actions)[4] = action_7_4;
(*row_7->actions)[PLS_ID] = action_7_PLS_ID;
(*row_7->actions)[PLS_DOUBLEQUOTED_STRING] = action_7_PLS_DOUBLEQUOTED_STRING;
row_7->gotos[12] = 21;
row_7->gotos[13] = 23;
rows.append(row_7);

ParsingTableRow *row_8 = new ParsingTableRow();
row_8->stateId = 8;
row_8->ownsActions = true;
(*row_8->actions)[PLS_ID] = action_8_PLS_ID;
(*row_8->actions)[PLS_DOUBLEQUOTED_STRING] = action_8_PLS_DOUBLEQUOTED_STRING;
row_8->gotos[12] = 26;
row_8->gotos[13] = 23;
rows.append(row_8);

ParsingTableRow *row_9 = new ParsingTableRow();
row_9->stateId = 9;
row_9->ownsActions = false;
(*row_9->actions)[PLS_ID] = action_8_PLS_ID;
(*row_9->actions)[PLS_DOUBLEQUOTED_STRING] = action_8_PLS_DOUBLEQUOTED_STRING;
row_9->gotos[12] = 27;
row_9->gotos[13] = 23;
rows.append(row_9);

ParsingTableRow *row_10 = new ParsingTableRow();
row_10->stateId = 10;
row_10->ownsActions = false;
(*row_10->actions)[PLS_ID] = action_8_PLS_ID;
(*row_10->actions)[PLS_DOUBLEQUOTED_STRING] = action_8_PLS_DOUBLEQUOTED_STRING;
row_10->gotos[12] = 28;
row_10->gotos[13] = 23;
rows.append(row_10);

ParsingTableRow *row_11 = new ParsingTableRow();
row_11->stateId = 11;
row_11->ownsActions = true;
(*row_11->actions)[75] = action_11_75;
rows.append(row_11);

ParsingTableRow *row_12 = new ParsingTableRow();
row_12->stateId = 12;
row_12->ownsActions = true;
(*row_12->actions)[23] = action_12_23;
row_12->gotos[11] = 30;
rows.append(row_12);

ParsingTableRow *row_13 = new ParsingTableRow();
row_13->stateId = 13;
row_13->ownsActions = true;
(*row_13->actions)[75] = action_13_75;
rows.append(row_13);

ParsingTableRow *row_14 = new ParsingTableRow();
row_14->stateId = 14;
row_14->ownsActions = true;
(*row_14->actions)[23] = action_14_23;
rows.append(row_14);

ParsingTableRow *row_15 = new ParsingTableRow();
row_15->stateId = 15;
row_15->ownsActions = true;
(*row_15->actions)[75] = action_15_75;
rows.append(row_15);

ParsingTableRow *row_16 = new ParsingTableRow();
row_16->stateId = 16;
row_16->ownsActions = true;
(*row_16->actions)[23] = action_16_23;
rows.append(row_16);

ParsingTableRow *row_17 = new ParsingTableRow();
row_17->stateId = 17;
row_17->ownsActions = true;
(*row_17->actions)[68] = action_17_68;
(*row_17->actions)[52] = action_17_52;
(*row_17->actions)[23] = action_17_23;
(*row_17->actions)[40] = action_17_40;
(*row_17->actions)[75] = action_17_75;
(*row_17->actions)[25] = action_17_25;
(*row_17->actions)[49] = action_17_49;
rows.append(row_17);

ParsingTableRow *row_18 = new ParsingTableRow();
row_18->stateId = 18;
row_18->ownsActions = true;
(*row_18->actions)[PLS_E_O_F] = action_18_PLS_E_O_F;
rows.append(row_18);

ParsingTableRow *row_19 = new ParsingTableRow();
row_19->stateId = 19;
row_19->ownsActions = true;
(*row_19->actions)[PLS_E_O_F] = action_19_PLS_E_O_F;
rows.append(row_19);

ParsingTableRow *row_20 = new ParsingTableRow();
row_20->stateId = 20;
row_20->ownsActions = false;
(*row_20->actions)[PLS_ANY] = action_6_PLS_ANY;
(*row_20->actions)[PLS_E_O_F] = action_6_PLS_E_O_F;
row_20->gotos[7] = 31;
rows.append(row_20);

ParsingTableRow *row_21 = new ParsingTableRow();
row_21->stateId = 21;
row_21->ownsActions = true;
(*row_21->actions)[PLS_ANY] = action_21_PLS_ANY;
(*row_21->actions)[PLS_E_O_F] = action_21_PLS_E_O_F;
rows.append(row_21);

ParsingTableRow *row_22 = new ParsingTableRow();
row_22->stateId = 22;
row_22->ownsActions = false;
(*row_22->actions)[PLS_ID] = action_8_PLS_ID;
(*row_22->actions)[PLS_DOUBLEQUOTED_STRING] = action_8_PLS_DOUBLEQUOTED_STRING;
row_22->gotos[12] = 32;
row_22->gotos[13] = 23;
rows.append(row_22);

ParsingTableRow *row_23 = new ParsingTableRow();
row_23->stateId = 23;
row_23->ownsActions = true;
(*row_23->actions)[PLS_ANY] = action_23_PLS_ANY;
(*row_23->actions)[PLS_DOT] = action_23_PLS_DOT;
(*row_23->actions)[PLS_E_O_F] = action_23_PLS_E_O_F;
rows.append(row_23);

ParsingTableRow *row_24 = new ParsingTableRow();
row_24->stateId = 24;
row_24->ownsActions = true;
(*row_24->actions)[PLS_ANY] = action_24_PLS_ANY;
(*row_24->actions)[PLS_DOT] = action_24_PLS_DOT;
(*row_24->actions)[PLS_E_O_F] = action_24_PLS_E_O_F;
rows.append(row_24);

ParsingTableRow *row_25 = new ParsingTableRow();
row_25->stateId = 25;
row_25->ownsActions = false;
(*row_25->actions)[PLS_ANY] = action_24_PLS_ANY;
(*row_25->actions)[PLS_DOT] = action_24_PLS_DOT;
(*row_25->actions)[PLS_E_O_F] = action_24_PLS_E_O_F;
rows.append(row_25);

ParsingTableRow *row_26 = new ParsingTableRow();
row_26->stateId = 26;
row_26->ownsActions = false;
(*row_26->actions)[PLS_ANY] = action_21_PLS_ANY;
(*row_26->actions)[PLS_E_O_F] = action_21_PLS_E_O_F;
rows.append(row_26);

ParsingTableRow *row_27 = new ParsingTableRow();
row_27->stateId = 27;
row_27->ownsActions = false;
(*row_27->actions)[PLS_ANY] = action_21_PLS_ANY;
(*row_27->actions)[PLS_E_O_F] = action_21_PLS_E_O_F;
rows.append(row_27);

ParsingTableRow *row_28 = new ParsingTableRow();
row_28->stateId = 28;
row_28->ownsActions = false;
(*row_28->actions)[PLS_ANY] = action_21_PLS_ANY;
(*row_28->actions)[PLS_E_O_F] = action_21_PLS_E_O_F;
rows.append(row_28);

ParsingTableRow *row_29 = new ParsingTableRow();
row_29->stateId = 29;
row_29->ownsActions = false;
(*row_29->actions)[PLS_ID] = action_8_PLS_ID;
(*row_29->actions)[PLS_DOUBLEQUOTED_STRING] = action_8_PLS_DOUBLEQUOTED_STRING;
row_29->gotos[12] = 34;
row_29->gotos[13] = 23;
rows.append(row_29);

ParsingTableRow *row_30 = new ParsingTableRow();
row_30->stateId = 30;
row_30->ownsActions = true;
(*row_30->actions)[75] = action_30_75;
rows.append(row_30);

ParsingTableRow *row_31 = new ParsingTableRow();
row_31->stateId = 31;
row_31->ownsActions = true;
(*row_31->actions)[PLS_E_O_F] = action_31_PLS_E_O_F;
rows.append(row_31);

ParsingTableRow *row_32 = new ParsingTableRow();
row_32->stateId = 32;
row_32->ownsActions = true;
(*row_32->actions)[PLS_ANY] = action_32_PLS_ANY;
(*row_32->actions)[PLS_E_O_F] = action_32_PLS_E_O_F;
rows.append(row_32);

ParsingTableRow *row_33 = new ParsingTableRow();
row_33->stateId = 33;
row_33->ownsActions = false;
(*row_33->actions)[PLS_ID] = action_8_PLS_ID;
(*row_33->actions)[PLS_DOUBLEQUOTED_STRING] = action_8_PLS_DOUBLEQUOTED_STRING;
row_33->gotos[13] = 35;
rows.append(row_33);

ParsingTableRow *row_34 = new ParsingTableRow();
row_34->stateId = 34;
row_34->ownsActions = false;
(*row_34->actions)[PLS_ANY] = action_32_PLS_ANY;
(*row_34->actions)[PLS_E_O_F] = action_32_PLS_E_O_F;
rows.append(row_34);

ParsingTableRow *row_35 = new ParsingTableRow();
row_35->stateId = 35;
row_35->ownsActions = true;
(*row_35->actions)[PLS_ANY] = action_35_PLS_ANY;
(*row_35->actions)[PLS_E_O_F] = action_35_PLS_E_O_F;
rows.append(row_35);

//--------------end parsing table code for target parser-------------

}
