select referenced_owner as owner, referenced_name as name, referenced_type as type, 
dependency_type, 
'user' as owner_icon_column,
replace(lower(referenced_type),' ','_') as obj_icon_column
from sys.all_dependencies where 
owner=:owner and name=:object_name and type=:object_type
order by owner, name
