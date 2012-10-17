select owner, index_name, index_type, uniqueness, status, table_owner, table_name from sys.all_indexes 
where table_owner=:owner and table_type='TABLE' order by index_name
