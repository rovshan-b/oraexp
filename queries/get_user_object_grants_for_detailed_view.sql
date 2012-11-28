select table_schema as owner, table_name as object_name, 
       rtrim (xmlagg (xmlelement (e, privilege || decode(grantable, 'YES', ' (GRANTABLE)', '') || ',')).extract ('//text()'), ',') granted_privileges,
       'user' as schema_icon_column
from sys.all_tab_privs where grantee=:owner
group by table_schema, table_name, grantee
order by table_schema, table_name
