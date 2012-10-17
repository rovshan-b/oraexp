select object_name as procedure_name, status from sys.all_objects where object_type='PROCEDURE' and owner=:owner order by object_name
