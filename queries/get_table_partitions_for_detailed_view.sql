select partition_name, composite, subpartition_count, high_value, partition_position, 
tablespace_name, pct_free, pct_used, ini_trans, max_trans, initial_extent, next_extent,
min_extent, max_extent, pct_increase, freelists, freelist_groups, logging, compression, 
num_rows, blocks, empty_blocks, avg_space, chain_cnt, avg_row_len, sample_size, last_analyzed, 
buffer_pool, global_stats, user_stats from sys.all_tab_partitions
where table_owner=:owner and table_name=:object_name
order by partition_position
