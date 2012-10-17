select object_name as function_name, status from sys.all_objects where object_type='FUNCTION' and owner=:owner order by object_name
