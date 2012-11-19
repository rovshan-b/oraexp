declare
    h1      NUMBER;         -- handle returned by OPEN
    th      NUMBER;         -- handle returned by ADD_TRANSFORM for DDL
    ddl     CLOB;           -- DDL
    
begin

 h1 := DBMS_METADATA.OPEN(:object_type);
 DBMS_METADATA.SET_FILTER(h1,'NAME', :object_name);
 DBMS_METADATA.SET_FILTER(h1,'SCHEMA', :owner);
 
 th := DBMS_METADATA.ADD_TRANSFORM(h1,'DDL');
 
 if :p_pretty = 'y' then
   DBMS_METADATA.SET_TRANSFORM_PARAM (th, 'PRETTY', TRUE);
 elsif :p_pretty = 'n' then
   DBMS_METADATA.SET_TRANSFORM_PARAM (th, 'PRETTY', FALSE);
 end if;
 
 if :p_sqlterm = 'y' then
   DBMS_METADATA.SET_TRANSFORM_PARAM (th, 'SQLTERMINATOR', TRUE);
 elsif :p_sqlterm = 'n' then
   DBMS_METADATA.SET_TRANSFORM_PARAM (th, 'SQLTERMINATOR', FALSE);
 end if;
 
 if :p_segattr = 'y' then
   DBMS_METADATA.SET_TRANSFORM_PARAM (th, 'SEGMENT_ATTRIBUTES', TRUE);
   
   if :p_storage = 'y' then
      DBMS_METADATA.SET_TRANSFORM_PARAM (th, 'STORAGE', TRUE);
   elsif :p_storage = 'n' then
      DBMS_METADATA.SET_TRANSFORM_PARAM (th, 'STORAGE', FALSE);
   end if;
   
   if :p_tablespace = 'y' then
      DBMS_METADATA.SET_TRANSFORM_PARAM (th, 'TABLESPACE', TRUE);
   elsif :p_tablespace = 'n' then
      DBMS_METADATA.SET_TRANSFORM_PARAM (th, 'TABLESPACE', FALSE);
   end if;
 
 elsif :p_segattr = 'n' then
   DBMS_METADATA.SET_TRANSFORM_PARAM (th, 'SEGMENT_ATTRIBUTES', FALSE);
 end if;
 
 if :p_cons = 'y' then
   DBMS_METADATA.SET_TRANSFORM_PARAM (th, 'CONSTRAINTS', TRUE);
   
   if :p_cons_as_alter = 'y' then
      DBMS_METADATA.SET_TRANSFORM_PARAM (th, 'CONSTRAINTS_AS_ALTER', TRUE);
   elsif :p_cons_as_alter = 'n' then
      DBMS_METADATA.SET_TRANSFORM_PARAM (th, 'CONSTRAINTS_AS_ALTER', FALSE);
   end if;
   
 elsif :p_cons = 'n' then
   DBMS_METADATA.SET_TRANSFORM_PARAM (th, 'CONSTRAINTS', FALSE);
 end if;
 
 if :p_ref_cons = 'y' then
   DBMS_METADATA.SET_TRANSFORM_PARAM (th, 'REF_CONSTRAINTS', TRUE);
 elsif :p_ref_cons = 'n' then
   DBMS_METADATA.SET_TRANSFORM_PARAM (th, 'REF_CONSTRAINTS', FALSE);
 end if;
 
 if :p_oid = 'y' then
   DBMS_METADATA.SET_TRANSFORM_PARAM (th, 'OID', TRUE);
 elsif :p_oid = 'n' then
   DBMS_METADATA.SET_TRANSFORM_PARAM (th, 'OID', FALSE);
 end if;
 
 if :p_size_byte_keyword = 'y' then
   DBMS_METADATA.SET_TRANSFORM_PARAM (th, 'SIZE_BYTE_KEYWORD', TRUE);
 elsif :p_size_byte_keyword = 'n' then
   DBMS_METADATA.SET_TRANSFORM_PARAM (th, 'SIZE_BYTE_KEYWORD', FALSE);
 end if;

 ddl := DBMS_METADATA.FETCH_CLOB(h1);
 
 DBMS_METADATA.CLOSE(h1);

 open :rs_out for select ddl from dual;
end;
