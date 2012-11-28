select tablespace_name, 
decode(max_bytes, -1 , 'Unlimited', 
  case 
     when max_bytes >= 1024*1024*1024 then to_char(round(max_bytes/1024/1024/1024, 2))||' G'
     when max_bytes >= 1024*1024 then to_char(round(max_bytes/1024/1024, 2))||' M'
     when max_bytes >= 1024 then to_char(round(max_bytes/1024, 2))||' K'
     else to_char(max_bytes) || ' B'
  end
) as quota, 
'tablespace' as icon_column from sys.dba_ts_quotas
where username=:owner and dropped='NO'
order by tablespace_name
