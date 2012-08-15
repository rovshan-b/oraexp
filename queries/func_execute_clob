procedure execute_clob(p_sql CLOB) is
  l_num        NUMBER := 0;
  l_upperbound NUMBER;
  l_sql DBMS_SQL.VARCHAR2S;
  l_cur INTEGER;
  l_ret NUMBER;
BEGIN
  l_upperbound := CEIL(DBMS_LOB.GETLENGTH(p_sql)/64);
  FOR i IN 1..l_upperbound
  LOOP
    l_sql(i) := DBMS_LOB.SUBSTR(p_sql ,64 ,((i-1)*64)+1);
  END LOOP;

  l_cur := DBMS_SQL.OPEN_CURSOR;
  DBMS_SQL.PARSE(l_cur, l_sql, 1, l_upperbound, FALSE, DBMS_SQL.NATIVE);
  l_ret := DBMS_SQL.EXECUTE(l_cur);
  dbms_sql.close_cursor(l_cur);
EXCEPTION WHEN OTHERS THEN
  if dbms_sql.is_open(l_cur) then
    dbms_sql.close_cursor(l_cur);
  end if;
  
  RAISE;
END;
