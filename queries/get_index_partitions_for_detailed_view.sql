select partition_name, composite, subpartition_count, high_value, partition_position, 
tablespace_name, pct_free, ini_trans, max_trans, initial_extent, next_extent,
min_extent, max_extent, pct_increase, freelists, freelist_groups, logging, compression, 
num_rows, sample_size, last_analyzed, 
buffer_pool, global_stats, user_stats from sys.all_ind_partitions
where index_owner=:owner and index_name=:object_name
order by partition_position
