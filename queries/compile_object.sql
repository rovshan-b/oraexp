declare
  l_object_name varchar2(50) := :object_name;
  l_owner varchar2(50) := :owner;
  l_object_type varchar2(50) := :object_type;
  l_compile_object_type varchar2(50) := :object_type;
  l_errors_object_type varchar2(50) := :object_type;
  
  l_for_debug number := :for_debug;
  
  l_enable_warnings number := :enable_warnings;
  
  l_native_code number := :native_code;
  
  l_compile_statement varchar2(500);
  
  {@replace_with:func_execute_clob}
begin 
  if l_object_type in ('PACKAGE SPEC', 'PACKAGE BODY') then
    l_compile_object_type := 'PACKAGE';
  elsif l_object_type in ('TYPE SPEC', 'TYPE BODY') then
    l_compile_object_type := 'TYPE';
  end if;

  l_compile_statement := 'alter '||l_compile_object_type||' "'||l_owner||'"."'||l_object_name||'" compile';
  if l_for_debug = 1 then
     l_compile_statement := l_compile_statement || ' debug';
  end if;
  
  if l_object_type in ('PACKAGE SPEC', 'TYPE SPEC') then
    l_compile_statement := l_compile_statement || ' specification';
    l_errors_object_type := substr(l_errors_object_type, 1, instr(l_errors_object_type,' ')-1);
  elsif l_object_type in ('PACKAGE BODY', 'TYPE BODY') then
    l_compile_statement := l_compile_statement || ' body';
  end if;
  
  if l_object_type !='VIEW' then
     if l_enable_warnings = 1 then
        l_compile_statement := l_compile_statement || ' PLSQL_WARNINGS=''ENABLE:ALL''';
     elsif l_enable_warnings = 0 then
        l_compile_statement := l_compile_statement || ' PLSQL_WARNINGS=''DISABLE:ALL''';
     end if;
     
     if l_for_debug != 1 then
        if l_native_code = 1 then
          l_compile_statement := l_compile_statement || ' PLSQL_CODE_TYPE=NATIVE';
        elsif l_native_code = 0 then
          l_compile_statement := l_compile_statement || ' PLSQL_CODE_TYPE=INTERPRETED';
        end if;
     end if;
  end if;

  execute_clob(l_compile_statement);
  
  open :rs_out for select line, position, text, 
                  {@keep_if:>=10}attribute{}
                  {@keep_if:<10}'ERROR' as attribute{}
                  from sys.all_errors  where owner=l_owner and name=l_object_name and type=l_errors_object_type order by sequence;
end;
