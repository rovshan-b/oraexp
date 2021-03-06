declare
    l_res_code clob;
    l_ix number;

    {@replace_with:func_trim_str}
    {@replace_with:func_get_source_with_new_owner}
begin
    l_res_code := trim_str(get_source_with_new_owner(:object_type, :owner, :object_name, :target_owner, 0, :wrap, :sql_terminator));
    l_res_code := substr(l_res_code, instr(l_res_code, 'CREATE'));
    
    /*
    if :object_type = 'VIEW' then
        l_ix:=instr(l_res_code,'CREATE OR REPLACE FORCE VIEW');
        if l_ix > 0 then
           l_res_code := 'CREATE OR REPLACE VIEW' || substr(l_res_code, length('CREATE OR REPLACE FORCE VIEW')+1);
        end if;
    end if;*/
    
    open :rs_out for select l_res_code as code from dual;
end;
