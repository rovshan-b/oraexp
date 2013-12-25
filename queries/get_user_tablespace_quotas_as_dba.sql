{@keep_if:>=10}
select tablespace_name, max_bytes from sys.dba_ts_quotas
where username=:owner and dropped='NO'
order by tablespace_name
{}
{@keep_if:<10}
select tablespace_name, max_bytes from sys.dba_ts_quotas
where username=:owner
order by tablespace_name
{}
