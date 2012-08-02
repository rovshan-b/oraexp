    procedure replace_source_owner(p_owner varchar2, p_new_owner varchar2, p_object_name varchar2, p_object_type varchar2, p_source in out clob) is
        l_owner_pos number;
    begin
        if p_owner != p_new_owner then
           
           l_owner_pos:=instr(p_source, '"'||p_owner||'"."'||p_object_name||'"');
           if l_owner_pos > 0 then
             p_source := substr(p_source, 1, l_owner_pos) || p_new_owner || '"' || substr(p_source, l_owner_pos + length(p_owner)+2);
           end if;
           
           
           if p_object_type = 'SYNONYM' then --call second time to replace owner of referenced object
             l_owner_pos:=instr(p_source, '"'||p_owner||'"."');
             if l_owner_pos > 0 then
                 p_source := substr(p_source, 1, l_owner_pos) || p_new_owner || '"' || substr(p_source, l_owner_pos + length(p_owner)+2);
             end if;
           end if;
        end if;
    end;
