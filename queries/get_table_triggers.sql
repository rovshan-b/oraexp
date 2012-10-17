select owner, trigger_name, trigger_type, triggering_event, status, base_object_type, table_owner, table_name from sys.all_triggers
where table_owner=:owner and table_name=:table_name and base_object_type='TABLE' order by trigger_name
