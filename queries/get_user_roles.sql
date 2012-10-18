select granted_role as privilege, admin_option, default_role as is_default from sys.dba_role_privs
where grantee=:owner
order by granted_role
