function get_source_code(p_object_type varchar2, p_owner varchar2, p_object_name varchar2, p_sql_terminator number := 1) return clob is
    h1      NUMBER;         -- handle returned by OPEN
    th      NUMBER;         -- handle returned by ADD_TRANSFORM for DDL
    ddl     CLOB;           -- DDL
begin
    h1 := DBMS_METADATA.OPEN(p_object_type);
    DBMS_METADATA.SET_FILTER(h1,'NAME', p_object_name);
    DBMS_METADATA.SET_FILTER(h1,'SCHEMA', p_owner);
    
    th := DBMS_METADATA.ADD_TRANSFORM(h1,'DDL');
    
    if p_sql_terminator = 1 then
      DBMS_METADATA.SET_TRANSFORM_PARAM (th, 'SQLTERMINATOR', TRUE);
    else
      DBMS_METADATA.SET_TRANSFORM_PARAM (th, 'SQLTERMINATOR', FALSE);
    end if;
    
    ddl := DBMS_METADATA.FETCH_CLOB(h1);
    
    DBMS_METADATA.CLOSE(h1);
    
    return ddl;
end;
