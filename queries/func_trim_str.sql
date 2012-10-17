    function trim_str(p_str varchar2) return varchar2 is
       l_result varchar2(4000);
    begin
       if p_str is null or length(p_str)=0 then
          return p_str;
       end if;    

       l_result := p_str;
       
       while substr(l_result, 1, 1) in (chr(13), chr(10), chr(32), chr(9), chr(11)) 
       loop
          l_result := substr(l_result, 2);          
       end loop;
       
       while substr(l_result, -1, 1) in (chr(13), chr(10), chr(32), chr(9), chr(11))
       loop
          l_result := substr(l_result, 1, length(l_result)-1);
       end loop;
       
       return l_result;
    end;

    function trim_str(p_str clob) return clob is
       l_result clob;
    begin
       if p_str is null or length(p_str)=0 then
          return p_str;
       end if;    

       l_result := p_str;
       
       while substr(l_result, 1, 1) in (chr(13), chr(10), chr(32), chr(9), chr(11)) 
       loop
          l_result := substr(l_result, 2);          
       end loop;
       
       while substr(l_result, -1, 1) in (chr(13), chr(10), chr(32), chr(9), chr(11))
       loop
          l_result := substr(l_result, 1, length(l_result)-1);
       end loop;
       
       return l_result;
    end;
