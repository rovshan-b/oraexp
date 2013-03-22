declare
  TYPE str_list_type IS TABLE OF VARCHAR2(50) INDEX BY BINARY_INTEGER;
  TYPE num_list_type IS TABLE OF NUMBER INDEX BY BINARY_INTEGER;
  
  l_table_names str_list_type := :table_names ;
  l_table_count number := :table_count ;
  l_schema varchar2(50) := :owner;
  
  l_self_ref_check_list str_list_type;
  l_self_ref_check_count number := 0;
  
  l_new_order num_list_type;
  
  l_fully_sorted number := 1;
  
  l_result clob;
  
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
                            p_checked_names in out str_list_type, p_checked_count in out number) return number is
  begin
     for cur_referenced in (select (select table_name from sys.all_constraints where owner=c1.r_owner and 
                            constraint_name=c1.r_constraint_name) as referenced_table_name
                            from sys.all_constraints c1 
                            where c1.owner=l_schema and c1.table_name=p_from_table and c1.constraint_type='R')
     loop
        if cur_referenced.referenced_table_name = p_to_table then
           return 1;
        else
           if is_in_collection(cur_referenced.referenced_table_name, p_checked_names, p_checked_count) = 0 then 
              p_checked_count := p_checked_count + 1;
              p_checked_names(p_checked_count) := cur_referenced.referenced_table_name;
              
              return reference_exists(p_from_table, p_to_table, p_checked_names, p_checked_count);
           end if;
        end if;
     end loop;
     
     return 0;
  end;
  
  function has_self_reference(p_table_name varchar2) return number is
     l_result number;
  begin
     select case when exists (      
            select 0 from (
               select (select table_name from sys.all_constraints where owner=c1.r_owner and 
               constraint_name=c1.r_constraint_name) as referenced_table_name
               from sys.all_constraints c1 
               where c1.owner=l_schema and c1.table_name=p_table_name and c1.constraint_type='R') t
            where t.referenced_table_name = p_table_name) then 1 else 0 end into l_result from dual;
  
     return l_result;
  end;
  
  procedure move_in_collection(p_item_to_move_up number, p_place_to_move number,
                               p_coll in out str_list_type, p_size number) is
     l_item_text_to_move varchar2(50);
     l_tmp_item varchar2(50);
  begin
     if p_item_to_move_up <= p_place_to_move then --supporting only moving items after p_place_to_move to p_place_to_move
        return;
     end if;
          
     l_tmp_item := p_coll(p_item_to_move_up);
     
     for i in reverse (p_place_to_move+1)..p_item_to_move_up
     loop
        p_coll(i) := p_coll(i-1);
     end loop;
     
     p_coll(p_place_to_move) := l_tmp_item;
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
                    
                    l_has_self_ref := has_self_reference(l_curr_table_name);
                    
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
  
  sort_tables;
  
  
  for i in 1..l_table_count
  loop
      l_result := l_result || l_table_names(i) ||',';
  end loop;
  
  open :rs_out for select l_result as sorted_names, l_fully_sorted as fully_sorted from dual;
end;
