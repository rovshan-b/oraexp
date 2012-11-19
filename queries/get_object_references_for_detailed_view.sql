select referenced_owner as owner, referenced_name as name, referenced_type as type, 
dependency_type, replace(lower(referenced_type),' ','_') as icon_name
from sys.all_dependencies where 
owner=:owner and name=:object_name and type=:object_type
order by owner, name
