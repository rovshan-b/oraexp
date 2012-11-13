select * from sys.all_triggers 
where base_object_type='TABLE' and 
table_owner=:owner and table_name=:object_name
order by owner, trigger_name
