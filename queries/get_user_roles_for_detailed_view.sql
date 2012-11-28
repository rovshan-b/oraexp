select granted_role, admin_option, default_role as is_default,
'users' as icon_column
from sys.dba_role_privs
where grantee=:owner
order by granted_role
