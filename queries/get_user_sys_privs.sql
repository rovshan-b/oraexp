select privilege, admin_option, 'NO' as is_default from sys.dba_sys_privs
where grantee=:owner
order by privilege
