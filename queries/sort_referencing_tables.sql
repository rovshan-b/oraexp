declare
  TYPE str_list_type IS TABLE OF VARCHAR2(50) INDEX BY BINARY_INTEGER;
  TYPE num_list_type IS TABLE OF NUMBER INDEX BY BINARY_INTEGER;
  TYPE obj_constraints_type IS RECORD
  (
     ref_table_names str_list_type,
     constraint_count number := 0
  );
  TYPE obj_constraint_list IS TABLE OF obj_constraints_type INDEX BY BINARY_INTEGER;
  
  l_table_names str_list_type := :table_names;
  l_table_count number := :table_count;
  l_schema varchar2(50) := :owner;
  
  l_tables_constraints obj_constraint_list;
  
  l_self_ref_check_list str_list_type;
  l_self_ref_check_count number := 0;
  
  l_new_order num_list_type;
  
  l_fully_sorted number := 1;
  
  l_result clob;
  
  procedure populate_constraints_list is
     l_count number;
  begin
     for i in 1..l_table_count
     loop         
         l_count := 0;
         for cur_constraints in (select (select table_name from sys.all_constraints where owner=c1.r_owner and 
                                constraint_name=c1.r_constraint_name) as referenced_table_name
                                from sys.all_constraints c1 
                                where c1.owner=l_schema and c1.table_name=l_table_names(i) and c1.constraint_type='R')
         loop
            l_count := l_count + 1;
            l_tables_constraints(i).ref_table_names(l_count) := cur_constraints.referenced_table_name;
         end loop;
         
         l_tables_constraints(i).constraint_count := l_count;
         
         --dbms_output.put_line('added '||l_count||' constraints to '||l_table_names(i));
     end loop;
  end;
  
  function get_table_index(p_table_name varchar2) return number is
  begin
     for i in 1..l_table_count
     loop  
        if l_table_names(i) = p_table_name then
           return i;
        end if;
     end loop;
     
     return 0;
  end;
  
  function is_in_collection(p_key varchar2, p_coll str_list_type, p_size number) return number is
  begin
     for i in 1..p_size
     loop
        if p_coll(i) = p_key then
           return 1;
        end if;
     end loop;
     
     return 0;
  end;
  
  function reference_exists(p_from_table varchar2, p_to_table varchar2, 
                            p_checked_names in out str_list_type, p_checked_count in out number,
                            p_recursive number := 1) return number is
     l_table_ix number;
     l_table_constraints obj_constraints_type;
     l_table_constraint_count number;
     l_ref_table_name varchar2(50);
  begin
     l_table_ix := get_table_index(p_from_table);
     l_table_constraints := l_tables_constraints(l_table_ix);
     l_table_constraint_count := l_table_constraints.constraint_count;
     
     for i in 1..l_table_constraint_count 
     loop
        if l_table_constraints.ref_table_names(i) = p_to_table then
           return 1;
        end if;
     end loop;
     
     if p_recursive = 0 then
        return 0;
     end if;
     
     for i in 1..l_table_constraint_count 
     loop
        l_ref_table_name := l_table_constraints.ref_table_names(i);
        if is_in_collection(l_ref_table_name, p_checked_names, p_checked_count) = 0 then 
           p_checked_count := p_checked_count + 1;
           p_checked_names(p_checked_count) := l_ref_table_name;
           
           return reference_exists(p_from_table, p_to_table, p_checked_names, p_checked_count);
        end if;
     end loop;
     
     return 0;
  end;
  
  procedure move_in_collection(p_item_to_move_up number, p_place_to_move number,
                               p_coll in out str_list_type, p_size number) is
     l_tmp_item varchar2(50);
     
     l_tmp_tab_const obj_constraints_type;
  begin
     if p_item_to_move_up <= p_place_to_move then --supporting only moving items after p_place_to_move to p_place_to_move
        return;
     end if;
          
     l_tmp_item := p_coll(p_item_to_move_up);
     l_tmp_tab_const := l_tables_constraints(p_item_to_move_up);
     
     for i in reverse (p_place_to_move+1)..p_item_to_move_up
     loop
        p_coll(i) := p_coll(i-1);
        l_tables_constraints(i) := l_tables_constraints(i-1);
     end loop;
     
     p_coll(p_place_to_move) := l_tmp_item;
     l_tables_constraints(p_place_to_move) := l_tmp_tab_const;
  end;
  
  procedure sort_tables is
     l_curr_table_name varchar2(50);
     l_next_table_name varchar2(50);
     l_did_changes number;
     
     l_has_fwd_ref number;
     l_has_back_ref number;
     l_has_self_ref number := 0;
     
     p_checked_names1 str_list_type;
     p_checked_count1 number := 0;
     
     p_checked_names2 str_list_type;
     p_checked_count2 number := 0;
  begin
     loop
        l_did_changes := 0;
        
        for i in 1..l_table_count
        loop
           l_curr_table_name := l_table_names(i);
           
           if i<l_table_count then
              for k in (i+1)..l_table_count
              loop
                 l_next_table_name := l_table_names(k);
                 
                 l_has_fwd_ref := reference_exists(l_curr_table_name, l_next_table_name, p_checked_names1, p_checked_count1);
                 l_has_back_ref := reference_exists(l_next_table_name, l_curr_table_name, p_checked_names2, p_checked_count2);
                 
                 if l_has_self_ref = 0 and
                         is_in_collection(l_curr_table_name, l_self_ref_check_list, l_self_ref_check_count) = 0 then
                    
                    l_self_ref_check_count := l_self_ref_check_count + 1;     
                    l_self_ref_check_list(l_self_ref_check_count) := l_curr_table_name;
                    
                    l_has_self_ref := reference_exists(l_curr_table_name, l_curr_table_name, p_checked_names2, p_checked_count2);
                    
                    if l_has_self_ref =1 then
                       l_fully_sorted := 0;
                    end if;
                 end if;
                 
                 if l_has_fwd_ref=1 and l_has_back_ref=0 then 
                    move_in_collection(k, i, l_table_names, l_table_count);
                    l_did_changes := 1;
                 elsif (l_has_fwd_ref=1 and l_has_back_ref=1) then --no circular dependency allowed
                    l_fully_sorted := 0;
                 end if;
              end loop;
           end if;
        end loop;
        
        exit when l_did_changes = 0;
     end loop;
  end;
  
begin

  populate_constraints_list;
  
  sort_tables;
  
  
  for i in 1..l_table_count
  loop
      l_result := l_result || l_table_names(i) ||',';
  end loop;
  
  open :rs_out for select l_result as sorted_names, l_fully_sorted as fully_sorted from dual;
end;
