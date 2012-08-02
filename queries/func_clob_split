    FUNCTION clob_split (p_clob IN CLOB) RETURN DBMS_SQL.VARCHAR2A IS   
      l_pos           PLS_INTEGER := 1;
      l_offset        PLS_INTEGER := 1;
      
      l_result        dbms_sql.varchar2a;

      l_ix            number := 1;
    BEGIN
        
        WHILE l_offset < DBMS_LOB.GETLENGTH(p_clob) LOOP
          l_result(l_ix) := DBMS_LOB.SUBSTR(p_clob,32767,l_offset);
          

          l_offset := l_offset + 32767;
          l_ix := l_ix + 1;
        END LOOP;

        RETURN l_result;

    END;
