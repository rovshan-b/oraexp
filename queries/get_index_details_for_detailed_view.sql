select * from sys.all_indexes 
where owner=:owner and index_name=:object_name
