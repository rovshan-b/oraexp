    procedure lob_replace( p_lob in out clob,
                       p_what in varchar2,
                       p_with in varchar2 )
    as
        n        number;
        l_offset number := 1;
    begin
       loop
           n := dbms_lob.instr( p_lob, p_what, l_offset );
           if ( nvl(n,0) > 0 )
           then
               if ( (n+length(p_what)) < dbms_lob.getlength(p_lob) )
               then
                  dbms_lob.copy( p_lob,
                                 p_lob,
                                 dbms_lob.getlength(p_lob),
                                 n+length(p_with),
                                 n+length(p_what) );
               end if;
                                                                                          
               dbms_lob.write( p_lob, length(p_with), n, p_with );
               if ( length(p_what) > length(p_with) )
               then
                   dbms_lob.trim( p_lob,
                      dbms_lob.getlength(p_lob)-(length(p_what)-length(p_with)) );
               end if;
               l_offset := l_offset + length(p_with);
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
