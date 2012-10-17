select distinct privilege from sys.dba_sys_privs
where grantee in ('DBA','SYS')
order by privilege
