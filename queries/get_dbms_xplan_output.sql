select * from table(dbms_xplan.display('PLAN_TABLE', :statement_id))
