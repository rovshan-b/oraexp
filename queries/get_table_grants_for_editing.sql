select table_schema, table_name, grantee,
       rtrim (xmlagg (xmlelement (e, privilege || decode(grantable, 'YES', ' (GRANTABLE)', '') || ',')).extract ('//text()'), ',') granted_privileges
from sys.all_tab_privs where table_schema=:owner and table_name=:object_name
group by table_schema, table_name,grantee