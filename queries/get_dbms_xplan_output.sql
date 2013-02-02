declare
   l_sql_id varchar2(50);
   l_child_number number;
begin
    if :autotrace = 0 then
        open :rs_out for select * from table(dbms_xplan.display('PLAN_TABLE', :statement_id));
    else
        execute immediate 
         'select SQL_ID, CHILD_NUMBER from v$sql where sql_text like ''/*OraExp:AutoTrace:''||:statement_id||''*/%'''
         into l_sql_id, l_child_number using :statement_id;
        
         open :rs_out for 'select * from table(dbms_xplan.display_cursor(:sql_id, :child_number))' using l_sql_id, l_child_number;
    end if;
end;
