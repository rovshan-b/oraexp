select subpartition_name, high_value, subpartition_position, 
tablespace_name, pct_free, ini_trans, max_trans, initial_extent, next_extent,
min_extent, max_extent, pct_increase, freelists, freelist_groups, logging, compression, 
num_rows, sample_size, last_analyzed, 
buffer_pool, global_stats, user_stats from sys.all_ind_subpartitions
where index_owner=:owner and index_name=:object_name and partition_name=:partition_name
order by subpartition_position
