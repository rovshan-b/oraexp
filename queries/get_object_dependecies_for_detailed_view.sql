select owner, name, type, dependency_type,
'user' as owner_icon_column,
replace(lower(type),' ','_') as obj_icon_column
from sys.all_dependencies where 
referenced_owner=:owner and referenced_name=:object_name and referenced_type=:object_type
order by owner, name
