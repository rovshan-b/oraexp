select grantor, grantee,
       rtrim (xmlagg (xmlelement (e, privilege || decode(grantable, 'YES', ' (GRANTABLE)', '') || ',')).extract ('//text()'), ',') granted_privileges,
       'user' as grantor_icon_column, 'user' as grantee_icon_column
from sys.all_tab_privs where table_schema=:owner and table_name=:object_name
group by grantee, grantor
