select line, position, text, attribute, lower(attribute) as icon_column from sys.all_errors 
where owner=:owner and name=:object_name and type=:object_type order by sequence
