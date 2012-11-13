select owner, name, type, dependency_type 
from sys.all_dependencies where 
referenced_owner=:owner and referenced_name=:object_name and referenced_type='TABLE'
order by owner, name
