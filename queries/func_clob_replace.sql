    procedure lob_replace( p_lob in out clob,
                       p_what in varchar2,
                       p_with in varchar2 )
    as
        n        number;
        l_offset number := 1;
        l_with_length number;
    begin
       l_with_length := nvl(length(p_with), 0);
 
       loop
           n := dbms_lob.instr( p_lob, p_what, l_offset );
           if ( nvl(n,0) > 0 )
           then
               if ( (n+length(p_what)) < dbms_lob.getlength(p_lob) )
               then
                  dbms_lob.copy( p_lob,
                                 p_lob,
                                 dbms_lob.getlength(p_lob),
                                 n+l_with_length,
                                 n+length(p_what) );
               end if;
               
               if l_with_length > 0 then                                                                           
                  dbms_lob.write( p_lob, l_with_length, n, p_with );
               end if;
               
               if ( length(p_what) > l_with_length )
               then
                   dbms_lob.trim( p_lob,
                      dbms_lob.getlength(p_lob)-(length(p_what)+l_with_length) );
               end if;
               l_offset := l_offset + l_with_length;
           else
               exit;
           end if;
       end loop;
    end;

    FUNCTION clob_replace
    ( p_clob          IN CLOB,
      p_what          IN VARCHAR2,
      p_with          IN VARCHAR2 ) RETURN CLOB IS
      l_result clob := p_clob;
    BEGIN
      lob_replace(l_result, p_what, p_with);
      return l_result;
    END;
