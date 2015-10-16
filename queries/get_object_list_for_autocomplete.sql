declare
	TYPE str_list_type IS TABLE OF VARCHAR2(50) INDEX BY BINARY_INTEGER;
	
	l_name_parts str_list_type := :name_parts;
    l_part_count number := :name_parts_count;
    
    l_default_schema varchar2(50) := :default_schema;
    
    l_prev_part_type varchar2(50);
    l_found number := 0;
    
    l_schema varchar2(50);
    l_object_type varchar2(50);
    l_object_name varchar2(50);
    
    l_cur_result SYS_REFCURSOR;
    
    procedure resolve_synonym(p_synonym_name varchar2, o_schema out varchar2, o_object_name out varchar2) is
    begin
		select table_owner, table_name into o_schema, o_object_name
			(select table_owner, table_name from sys.all_synonyms where synonym_name = p_synonym_name
				order by (case when owner = l_default_schema then 1 else 2 end)) t
		where rownum <= 1;
	exception when no_data_found then
		o_schema := null;
		o_object_name := null;
    end;
    
    procedure find_object_type(p_object_name varchar2, o_schema out varchar2, o_object_type out varchar2) is
		l_syn_obj_name varchar2(50);
    begin
		select owner, object_type into o_schema, o_object_type  from
			(select owner, object_type from sys.all_objects where object_name = p_object_name
				order by (case when owner = l_default_schema then 1 else 2 end) ) t 
		where rownum <= 1
		
		if o_object_type = 'SYNONYM' then
			resolve_synonym(p_object_name, o_schema, l_syn_obj_name);
			if l_syn_obj_name is null then
				o_schema := null;
				o_object_type := null;
			end if;
		end if;
		
	exception when no_data_found then
		o_schema := null;
		o_object_type := null;
    end;
    
    procedure find_object_type_in_schema(p_object_name varchar2, p_schema varchar2, o_object_type out varchar2) is
		l_syn_obj_name varchar2(50);
    begin
		select object_type into o_object_type from
			(select object_type from sys.all_objects where owner = p_schema and object_name = p_object_name 
				order by length(object_type) --find "package" before "package body"
			) 
		t where rownum <= 1;
		
		if o_object_type = 'SYNONYM' then
			resolve_synonym(p_object_name, o_schema, l_syn_obj_name);
			if l_syn_obj_name is null then
				o_schema := null;
				o_object_type := null;
			end if;
		end if;
		
	exception when no_data_found then
		o_object_type := null;
    end;
    
    procedure find_last_part_object_type() is
    begin
		for i in 1..l_part_count
		loop
			if i = 1 then
				--check if there is an object visible from current schema
				find_object_type(upper(l_name_parts(i)), l_schema, l_object_type);
				
				if l_schema is null then --no top level object visible from current schema
					--check if it is a schema name
					select case when exists (select 0 from sys.all_users where username=upper(l_name_parts(i))) 
					then 1 else 0 end into l_found from dual;
					
					if l_found = 1 then
						l_schema := upper(l_name_parts(i));
						l_object_type := 'USER';
					else
						--could not find anything. need to return empty result from here
						RETURN;
					end if;
				else --if schema is not null, it means first part is already existing object name visible from current schema
					find_object_type_in_schema(upper(l_name_parts(i)), l_schema, l_object_type);
				end if;
			else -- i>1
				if l_object_type is null then --could not detect object type at some step
					--could not find anything. need to return empty result from here
					RETURN;
				end if;
			
				find_object_type_in_schema(upper(l_name_parts(i)), l_schema, l_object_type);
			end if;
			
			l_object_name := l_name_parts(l_part_count);
		end loop;
    end;
begin
	find_last_part_object_type();
	
	if l_object_type is not null then --found all object types until last part
		if l_object_type = 'USER' then
			open l_cur_result for select object_name from sys.all_objects where owner = l_schema;
		elsif l_object_type = 'TABLE' then
			open l_cur_result for select column_name from sys.all_tab_columns cols where owner=l_schema and table_name=l_object_name order by column_id 
		elsif l_object_type = 'PACKAGE' then
			--get package functions and procedures
		elsif l_object_type = 'SEQUENCE' then
			--select currval, nextval
		else
			--select empty resultset
		end if;
	end if;
end;
