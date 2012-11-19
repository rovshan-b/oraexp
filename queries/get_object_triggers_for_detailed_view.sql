select owner, trigger_name, trigger_type,  
triggering_event, status, column_name, 
referencing_names, when_clause,
'trigger' as icon_column
from sys.all_triggers 
where base_object_type=:object_type and 
table_owner=:owner and table_name=:object_name
order by owner, trigger_name
