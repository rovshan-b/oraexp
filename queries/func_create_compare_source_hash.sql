    function create_source_hash(p_object_type varchar2, p_owner varchar2, p_object_name varchar2, p_new_owner varchar2, p_remove_whitespace number, p_wrap number) return varchar2 is
       l_hash varchar2(50);
    begin
       l_hash := md5(get_source_with_new_owner(p_object_type, p_owner, p_object_name, p_new_owner, p_remove_whitespace, p_wrap));
       
       return l_hash;
    end;
    
    function compare_source_hash(p_object_type varchar2, p_owner varchar2, p_object_name varchar2, p_new_owner varchar2, p_hash varchar2, p_remove_whitespace number, p_wrap number) return varchar2 is
    begin     
      if p_hash = create_source_hash(p_object_type, p_owner, p_object_name, p_new_owner, p_remove_whitespace, p_wrap) then
        return 'same';
      else
        return 'different';
      end if;
    end;
