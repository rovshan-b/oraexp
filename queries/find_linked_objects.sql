declare
  TYPE str_list_type IS TABLE OF VARCHAR2(50) INDEX BY BINARY_INTEGER;
  TYPE long_str_list_type IS TABLE OF VARCHAR2(32767) INDEX BY BINARY_INTEGER;
  obj_names str_list_type := :obj_names;
  obj_types str_list_type := :obj_types;
  obj_count number := :obj_count;
  
  scan_obj_types str_list_type := :scan_obj_types;
  scan_obj_count number := :scan_obj_count;
  
  collected_obj_names str_list_type;
  collected_obj_types str_list_type;
  collected_obj_levels str_list_type;
  collected_ref_types long_str_list_type;
  collected_obj_count number := 0;
  
  l_schema varchar2(50) := :owner;
  l_name_like varchar2(50) := :name_like;
  l_name_not_like varchar2(50) := :name_not_like;
  l_max_level number := :max_level;
  l_scan_type number := :scan_type; --0 - dependent objects, 1 - referenced objects, 2 - dependents and references 
  l_only_tables number := :only_tables;
  l_stop_on_invalid_type number := :stop_on_invalid_type;
  l_only_referenced_after_level number := :only_referenced_after_level;
  
  l_query varchar2(4000);
  
  l_current_level number := 0;
  
  l_result clob;
   
  c_dependent_obj_query constant varchar2(4000) := 'select name, type, ''Depends on '' || :object_name as ref_type from sys.all_dependencies where owner = :owner and referenced_owner = :owner and referenced_name = :object_name and referenced_type = :object_type';
  c_referenced_obj_query constant varchar2(4000) := 'select referenced_name as name, referenced_type as type, ''Referenced by '' || :object_name as ref_type from sys.all_dependencies where owner = :owner and referenced_owner = :owner and name = :object_name and type = :object_type';
  c_both_query constant varchar2(4000) := c_dependent_obj_query || ' union ' || c_referenced_obj_query;
                     
  c_dependent_tables_query constant varchar2(4000) := 'select table_name as name, ''TABLE'' as type, ''FK to '' || :object_name as ref_type
                                                       from sys.all_constraints where r_owner=:owner and constraint_type=''R'' and
                                                       r_constraint_name in (select constraint_name from sys.all_constraints 
                                                       where owner=:owner and table_name=:object_name and constraint_type=''P'')';
  c_referenced_tables_query constant varchar2(4000) := 'select (select table_name from sys.all_constraints where owner=c1.r_owner and constraint_name=c1.r_constraint_name) as name, ''TABLE'' as type, ''FK from '' || :object_name as ref_type
                                                        from sys.all_constraints c1 
                                                        where c1.owner=:owner and c1.table_name=:object_name and c1.constraint_type=''R''';
  c_both_tables_query constant varchar2(4000) := c_dependent_tables_query || ' union ' || c_referenced_tables_query;
  
  function is_in_original_list(p_obj_name varchar2, p_obj_type varchar2) return number is
  begin
     for i in 1..obj_count 
     loop
        if obj_names(i) = p_obj_name then
           return 1;
        end if;
        
        return 0;
     end loop;
  end;
  
  function is_valid_type(p_obj_type varchar2) return number is
  begin
     if p_obj_type is null then
        return 0;
     end if;
     
     for i in 1..scan_obj_count 
     loop
        if scan_obj_types(i) = p_obj_type then
           return 1;
        end if;
        
        if (p_obj_type='PACKAGE BODY' and scan_obj_types(i)='PACKAGE') or
           (p_obj_type='TYPE BODY' and scan_obj_types(i)='TYPE') then
              return 1;
        end if;
     end loop;
     
     return 0;
  end;
  
  function already_collected(p_obj_name varchar2, p_obj_type varchar2, p_ref_type varchar2) return number is
  begin
     if p_obj_name is null then
        return 1;
     end if;
  
     for i in 1..collected_obj_count 
     loop
        if collected_obj_names(i) = p_obj_name and collected_obj_types(i) = p_obj_type then
           if instr(collected_ref_types(i), p_ref_type||'^') = 0 then
              if length(collected_ref_types(i))>5100 then
                null;
              elsif length(collected_ref_types(i))>5000 then
                collected_ref_types(i) := collected_ref_types(i) || '...^';
              else
                collected_ref_types(i) := collected_ref_types(i) || p_ref_type ||'^';
              end if;
           end if;
           
           return 1;
        end if;
     end loop;
     
     return 0;
  end;
  
  procedure add_to_collected(p_obj_name varchar2, p_obj_type varchar2, p_ref_type varchar2) is
  begin
     if p_obj_name is null then
        return;
     end if;
  
     collected_obj_count := collected_obj_count + 1;
     collected_obj_names(collected_obj_count) := p_obj_name;
     collected_obj_types(collected_obj_count) := p_obj_type;
     collected_obj_levels(collected_obj_count) := l_current_level;
     collected_ref_types(collected_obj_count) := p_ref_type ||'^';
  end;
  
  function modify_query(p_query varchar2, p_scan_type varchar2, p_obj_type varchar2) return varchar2 is
     l_query_to_append varchar2(4000);
  begin
     if p_obj_type != 'TABLE' then
        return p_query;
     end if;
     
     if p_scan_type = 0 then --objects that depend on this object
        l_query_to_append := c_dependent_tables_query;
     elsif p_scan_type = 1 then --objects that this object depends on (references them)
        l_query_to_append := c_referenced_tables_query;
     else
        l_query_to_append := c_both_tables_query;
     end if; 
     
     if l_only_tables = 1 then
        return l_query_to_append;
     else
        return p_query || ' union ' || l_query_to_append;
     end if;
  end;
  
  procedure collect_dependencies(p_obj_name varchar2, p_obj_type varchar2) is  
      l_name varchar2(50);
      l_type varchar2(50);
      l_ref_type varchar2(100);
      
      names_queue str_list_type;
      types_queue str_list_type;
      queue_size number := 0;
      
      l_cursor_id integer;
      l_dummy integer;
      
      l_curr_scan_type number := l_scan_type;
      
      procedure add_to_queue(p_obj_name varchar2, p_obj_type varchar2) is
      begin
         if p_obj_name is null then
            return;
         end if;
      
         queue_size := queue_size + 1;
         names_queue(queue_size) := p_obj_name;
         types_queue(queue_size) := p_obj_type;
      end;
  begin
      l_current_level := l_current_level+1;
  
      if l_max_level > 0 and l_current_level > l_max_level then
         l_current_level := l_current_level-1;
         return;
      end if;
      
      if l_scan_type = 2 and l_only_referenced_after_level>0 and l_current_level > l_only_referenced_after_level then
         l_curr_scan_type := 1; --if level is more than 1, fetch only objects that current object depends on
         l_query := c_referenced_obj_query;
      else
         if l_scan_type = 0 then --objects that depend on this object
            l_query := c_dependent_obj_query;
         elsif l_scan_type = 1 then --objects that this object depends on (references them)
            l_query := c_referenced_obj_query;
         else
            l_query := c_both_query;
         end if; 
      end if;
      
      l_query := modify_query(l_query, l_curr_scan_type, p_obj_type);
      
      l_cursor_id := dbms_sql.open_cursor; 
        
      dbms_sql.parse(l_cursor_id, l_query, dbms_sql.native);
      
      dbms_sql.bind_variable(l_cursor_id, ':owner', l_schema);       
      dbms_sql.bind_variable(l_cursor_id, ':object_name', p_obj_name);       
      if l_only_tables = 0 then
         dbms_sql.bind_variable(l_cursor_id, ':object_type', p_obj_type);       
      end if;
      
      dbms_sql.define_column(l_cursor_id, 1, l_name, 50); 
      dbms_sql.define_column(l_cursor_id, 2, l_type, 50); 
      dbms_sql.define_column(l_cursor_id, 3, l_ref_type, 100); 
      
      l_dummy := dbms_sql.execute(l_cursor_id); 
      
      LOOP 
          if dbms_sql.fetch_rows(l_cursor_id)>0 then 
            -- get column values of the row 
            dbms_sql.column_value(l_cursor_id, 1, l_name); 
            dbms_sql.column_value(l_cursor_id, 2, l_type); 
            dbms_sql.column_value(l_cursor_id, 3, l_ref_type); 
            
            if already_collected(l_name, l_type, l_ref_type) = 0 and (l_stop_on_invalid_type = 0 or is_valid_type(l_type) = 1) then
               add_to_collected(l_name, l_type, l_ref_type);
               add_to_queue(l_name, l_type);
               
               if l_type = 'PACKAGE' then
                  add_to_collected(l_name, 'PACKAGE BODY', l_ref_type);
                  add_to_queue(l_name, 'PACKAGE BODY');
               elsif l_type = 'TYPE' then
                  add_to_collected(l_name, 'TYPE BODY', l_ref_type);
                  add_to_queue(l_name, 'TYPE BODY');
               end if;       
             end if;
             
          else 
            exit; 
          end if; 
      END LOOP; 
      
      dbms_sql.close_cursor(l_cursor_id); 
      
      for k in 1..queue_size
      loop
         collect_dependencies(names_queue(k), types_queue(k));
      end loop;
      
      l_current_level := l_current_level-1;
  exception when others then 
       --dbms_output.put_line(dbms_utility.format_error_backtrace);
       if dbms_sql.is_open(l_cursor_id) then 
         dbms_sql.close_cursor(l_cursor_id); 
       end if; 
       raise;
  end;
begin  
   if l_name_like is null then
      l_name_like := '%';
   end if;
   
   if l_name_not_like is null then
      l_name_not_like := ' ';
   end if;
      
   for i in 1..obj_count
   loop      
      collect_dependencies(obj_names(i), obj_types(i));
   end loop;
   
   for i in 1..collected_obj_count
   loop
      if collected_obj_names(i) like l_name_like and 
         collected_obj_names(i) not like l_name_not_like and 
         is_valid_type(collected_obj_types(i))=1 and
         is_in_original_list(collected_obj_names(i), collected_obj_types(i)) = 0 then
           l_result := l_result || collected_obj_names(i)||','||collected_obj_types(i)||','||collected_obj_levels(i)||','||collected_ref_types(i)||';';
      end if;
   end loop;
   
   open :rs_out for select l_result from dual;
end;
