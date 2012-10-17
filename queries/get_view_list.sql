select object_name as view_name, status from sys.all_objects where object_type='VIEW' and owner=:owner and (:view_name is null or object_name=:view_name) order by view_name
