select * from sys.all_ind_statistics
where owner=:owner and index_name=:object_name
