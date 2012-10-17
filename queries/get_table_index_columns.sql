declare 
  l_ix_columns clob; 
  l_col_expression long;  
begin

    for cc in (select column_name, column_position
               from sys.all_ind_columns aic where 
               index_owner=:index_owner and index_name=:index_name and table_owner=:owner and table_name=:table_name
               order by column_position)
    loop
        begin
         select column_expression into l_col_expression from sys.all_ind_expressions
         where index_owner=:index_owner and index_name=:index_name and table_owner=:owner and table_name=:table_name and
         column_position=cc.column_position;
        exception when no_data_found then
         l_col_expression := null;
        end;

        l_ix_columns := l_ix_columns || nvl(l_col_expression, cc.column_name) || ',';
    end loop;
    
    l_ix_columns := substr(l_ix_columns, 1, length(l_ix_columns)-1);

    open :rs_out for select l_ix_columns from dual;
end;
