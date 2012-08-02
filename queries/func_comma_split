    FUNCTION comma_split(p_list IN VARCHAR2) RETURN t_tbl_type
    AS
        l_string       VARCHAR2(32767) := p_list || ',';
        l_comma_index  PLS_INTEGER;
        l_index        PLS_INTEGER := 1;
        l_tab          t_tbl_type ;--:= t_tbl_type();
     BEGIN
       LOOP
          l_comma_index := INSTR(l_string, ',', l_index);
          EXIT WHEN l_comma_index = 0;
          --l_tab.EXTEND;
          l_tab(l_tab.COUNT) := SUBSTR(l_string, l_index, l_comma_index - l_index);
          l_index := l_comma_index + 1;
        END LOOP;
         RETURN l_tab;
     END;
