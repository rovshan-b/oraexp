declare
	 TYPE str_list_type IS TABLE OF VARCHAR2(50) INDEX BY BINARY_INTEGER;
	
	 --l_name_parts string_list := pkg_util.get_string_table(:name_parts);
	 l_name_parts str_list_type := :name_parts;
    l_part_count number := :name_parts_count;
    
    l_default_schema varchar2(50) := :default_schema;
    l_ends_with_separator number := :ends_with_separator;
    
    l_prev_part_type varchar2(50);
    l_found number := 0;
    
    l_schema varchar2(50);
    l_object_type varchar2(50);
    l_object_name varchar2(50);
    
    l_cur_result SYS_REFCURSOR;
    
    procedure log(p_text varchar2) is
    begin
       --dbms_output.put_line(p_text);
       null;
    end;
    
    procedure resolve_synonym(p_synonym_name varchar2, o_schema out varchar2, o_object_name out varchar2) is
    begin
       log('resolving synonym');
		select table_owner, table_name into o_schema, o_object_name from
			(select table_owner, table_name from sys.all_synonyms where synonym_name = p_synonym_name
				order by (case when owner = l_default_schema then 1 else 2 end)) t
		where rownum <= 1;
		log('resolved to '||o_object_name);
		l_object_name := o_object_name;
	exception when no_data_found then
		o_schema := null;
		o_object_name := null;
		log('could not resolve');
    end;
    
    procedure find_object_type(p_object_name varchar2, o_schema in out varchar2, o_object_type out varchar2) is
		l_syn_obj_name varchar2(50);
    begin
       log('1. looking for object named '||p_object_name||' in schema '||o_schema);
		select owner, object_type into o_schema, o_object_type  from
			(select owner, object_type from sys.all_objects where object_name = p_object_name
				order by (case when owner = l_default_schema then 1 else 2 end) ) t 
		where rownum <= 1;
		log('1. type is '||o_object_type);
		if o_object_type = 'SYNONYM' then
			resolve_synonym(p_object_name, o_schema, l_syn_obj_name);
			if l_syn_obj_name is null then
				o_schema := null;
				o_object_type := null;
			else
			   find_object_type(l_syn_obj_name, o_schema, o_object_type);
			end if;
		end if;
		
	exception when no_data_found then
		o_schema := null;
		o_object_type := null;
    end;
    
    procedure find_object_type_in_schema(p_object_name varchar2, o_schema in out varchar2, o_object_type out varchar2) is
		l_syn_obj_name varchar2(50);
    begin
       log('2. looking for object named '||p_object_name||' in schema '||o_schema);
		select object_type into o_object_type from
			(select object_type from sys.all_objects where owner = o_schema and object_name = p_object_name 
				order by length(object_type) --find "package" before "package body"
			) 
		t where rownum <= 1;
		log('2. type is '||o_object_type);
		if o_object_type = 'SYNONYM' then
			resolve_synonym(p_object_name, o_schema, l_syn_obj_name);
			if l_syn_obj_name is null then
				o_schema := null;
				o_object_type := null;
			else
			   find_object_type_in_schema(l_syn_obj_name, o_schema, o_object_type);
			end if;
		end if;
		
	exception when no_data_found then
		o_object_type := null;
    end;
    
    procedure find_last_part_object_type is
    begin
		--for i in 1..l_part_count
		for i in 1..l_part_count
		loop
		    l_object_name := upper(l_name_parts(l_part_count));
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
						exit;
					end if;
				else --if schema is not null, it means first part is already existing object name visible from current schema
				   find_object_type_in_schema(upper(l_name_parts(i)), l_schema, l_object_type);
				end if;
			else -- i>1
				if l_object_type is null then --could not detect object type at some step
					--could not find anything. need to return empty result from here
					exit;
				end if;
			    
				find_object_type_in_schema(upper(l_name_parts(i)), l_schema, l_object_type);
			end if;
		end loop;
    end;
begin
   if l_ends_with_separator = 1 then
	   find_last_part_object_type;
   end if;
	
	if l_object_type is not null and l_ends_with_separator = 1 then --found all object types until last part
		if l_object_type = 'USER' then
			open l_cur_result for select lower(object_name) as object_name, owner, object_type, icon_name from (
                            select table_name as object_name, owner, 'TABLE' as object_type, 'table' as icon_name from sys.all_all_tables where owner = l_object_name
                            union all
                            select lower(object_name), owner, object_type, replace(lower(object_type),' ','_') as icon_name
                            from sys.all_objects
                            where object_type in ('VIEW','MATERIALIZED VIEW','SEQUENCE','PACKAGE','PROCEDURE','FUNCTION','JAVA CLASS','SYNONYM') and owner = l_object_name
                            union all
                            select lower(type_name) as object_name, owner, 'TYPE' as object_type, 'type' as icon_name 
                            from sys.all_types where typecode='OBJECT' and owner = l_object_name
                            ) t where object_name not like '/%' order by object_name;
		elsif l_object_type in ('TABLE','VIEW') then
		    log('looking for columns of table named '||l_object_name||' in schema '||l_schema);
			open l_cur_result for select lower(column_name) as object_name, owner, 'COLUMN' as object_type, 'column' as icon_name from sys.all_tab_columns cols where owner=l_schema and table_name=l_object_name order by column_id;
		elsif l_object_type = 'PACKAGE' then
			--get package functions and procedures
			open l_cur_result for select lower(p.procedure_name) as object_name,
			                             p.owner,
			                             decode (trim (a.position), null, 'PROCEDURE',
			                                     'FUNCTION'
			                                    ) as object_type,
			                             decode (a.position, null, 'procedure',
			                                     'function'
			                                    ) as icon_name
			                         from sys.all_procedures p
			                         left join sys.all_arguments a on (p.owner = a.owner
			                                                           and p.object_name = a.package_name
			                                                           and p.subprogram_id = a.subprogram_id
			                                                           and a.position = 0)
			                         where p.owner = l_schema
			                         and p.object_name = l_object_name
			                         and p.procedure_name is not null
			                         order by p.procedure_name;
		elsif l_object_type = 'SEQUENCE' then
			--select currval, nextval
			open l_cur_result for select 'currval' from dual
			                      union
			                      select 'nextval' from dual;
		else
			--select empty resultset
			null;
		end if;
		
	end if;
	
	if l_ends_with_separator = 0 then
	   open l_cur_result for select lower(object_name) as object_name, owner, object_type, icon_name from (
                            select table_name as object_name, owner, 'TABLE' as object_type, 'table' as icon_name from sys.all_all_tables where owner = l_default_schema
                            union all
                            select lower(object_name), owner, object_type, replace(lower(object_type),' ','_') as icon_name
                            from sys.all_objects
                            where object_type in ('VIEW','MATERIALIZED VIEW','SEQUENCE','PACKAGE','PROCEDURE','FUNCTION','JAVA CLASS','SYNONYM') and owner = l_default_schema
                            union all
                            select lower(type_name) as object_name, owner, 'TYPE' as object_type, 'type' as icon_name
                            from sys.all_types where typecode='OBJECT' and owner = l_default_schema
                            --union all
                            --select lower(synonym_name), owner, 'SYNONYM', 'synonym' from all_synonyms where owner='PUBLIC'
                            ) t where object_name not like '/%' order by object_name;
	end if;
	
	if l_object_name is null and l_ends_with_separator = 1 then
		  --open l_cur_result for select 'nothing selected' from dual;
		  open l_cur_result for select null as object_name, null as owner, null as object_type, null as icon_name from dual;
	end if;
		
	:rs_out := l_cur_result;
end;
