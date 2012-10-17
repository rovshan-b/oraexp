select owner, trigger_name, trigger_type, triggering_event, status, base_object_type, table_owner, table_name from sys.all_triggers
where table_owner=:owner order by trigger_name
