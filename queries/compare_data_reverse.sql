declare
  l_record_exists number;
  l_dml clob; --returned if working in generate query mode
  l_delete_sql clob;
  l_delete_count number := 0;
  
  {@replace_with:func_execute_clob}
begin
  for i in 1 .. :arr_size loop
    l_delete_sql := null;
    
    select (case when exists (select 0 from {source_table}
                              where ({uq_columns_and_coll_all}))
            then 1 else 0 end) 
    into l_record_exists from dual;
  
    if l_record_exists = 0 then
      l_delete_sql := 'delete from {dyn_target_table} where ({uq_columns_and_coll_values})';
      
      l_dml := l_dml || l_delete_sql || ';' || chr(10);
    
      l_delete_count := l_delete_count + 1;
    
    end if;
    
  end loop;
  
  :delete_count := l_delete_count;
  
  open :rs_out for select l_dml from dual;
  
end;