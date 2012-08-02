    FUNCTION clob_replace
    ( p_clob          IN CLOB,
      p_what          IN VARCHAR2,
      p_with          IN VARCHAR2 := NULL ) RETURN CLOB IS

      c_whatLen       CONSTANT PLS_INTEGER := LENGTH(p_what);
      c_withLen       PLS_INTEGER := LENGTH(p_with);

      l_return        CLOB;
      l_segment       CLOB;
      l_pos           PLS_INTEGER := 1-c_withLen;
      l_offset        PLS_INTEGER := 1;

    BEGIN
      if c_withLen is null then
        c_withLen := 0;
        l_pos := 1-c_withLen;
      end if;

      IF p_what IS NOT NULL THEN
        WHILE l_offset < DBMS_LOB.GETLENGTH(p_clob) LOOP
          l_segment := DBMS_LOB.SUBSTR(p_clob,32767,l_offset);
          LOOP
            l_pos := DBMS_LOB.INSTR(l_segment,p_what,l_pos+c_withLen);
            EXIT WHEN (NVL(l_pos,0) = 0) OR (l_pos = 32767-c_withLen);
            l_segment := TO_CLOB( DBMS_LOB.SUBSTR(l_segment,l_pos-1)
                                ||p_with
                                ||DBMS_LOB.SUBSTR(l_segment,32767-c_whatLen-l_pos-c_whatLen+1,l_pos+c_whatLen));
          END LOOP;
          l_return := l_return||l_segment;
          l_offset := l_offset + 32767 - c_whatLen;
        END LOOP;
      END IF;

      RETURN(l_return);

    END;
