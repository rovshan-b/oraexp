select owner, name, type, dependency_type,
replace(lower(type),' ','_') as icon_name
from sys.all_dependencies where 
referenced_owner=:owner and referenced_name=:object_name and referenced_type=:object_type
order by owner, name
