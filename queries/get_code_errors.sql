select line, position, text, attribute from sys.all_errors where owner=:owner and name=:object_name and type=:object_type order by sequence
