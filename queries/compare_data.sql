declare
   l_dml clob; --returned if working in generate query mode
   l_update_sql clob;
   l_row_exists number;
   l_insert_count number := 0;
   l_update_count number := 0;
   
   l_generate_inserts number := :generate_inserts;
   l_generate_updates number := :generate_updates;
   l_update_database number := :update_database;
   
   
   {col_var_declarations}
     
   {@replace_with:func_execute_clob}
begin
   for i in 1 .. :arr_size loop
      l_update_sql := null;
      
      begin
        select {col_names_for_select_into} into {col_var_names}
        from {target_table} where ({uq_columns_and_coll_all});
        
        l_row_exists := 1;
      exception when no_data_found then
        l_row_exists := 0;
      end;
      
      if l_row_exists = 1 and l_generate_updates = 1 then --generate update query
        {compare_col_values_script}
        if l_update_sql is not null then
            l_update_sql := 'update {dyn_target_table} set ' || l_update_sql || ' where ({uq_columns_and_coll_values})';

            if l_update_database = 1 then
              execute_clob(l_update_sql);
            else
              l_dml := l_dml || l_update_sql || ';' || chr(10);
            end if;
            
            l_update_count := l_update_count + 1;
        end if;
      elsif l_generate_inserts = 1 then --generate insert query
      
        l_update_sql := 'insert into {dyn_target_table} ({col_names}) values ({all_coll_values})';
        if l_update_database = 1 then
          execute_clob(l_update_sql);
        else
          l_dml := l_dml || l_update_sql || ';' || chr(10);
        end if;
        
        l_insert_count := l_insert_count + 1;
      end if;
   end loop;
   
   :insert_count := l_insert_count;
   :update_count := l_update_count;
   
   commit;

   open :rs_out for select l_dml from dual;
   
end;
