select subpartition_name, high_value, subpartition_position, 
tablespace_name, pct_free, pct_used, ini_trans, max_trans, initial_extent, next_extent,
min_extent, max_extent, pct_increase, freelists, freelist_groups, logging, compression, 
num_rows, blocks, empty_blocks, avg_space, chain_cnt, avg_row_len, sample_size, last_analyzed, 
buffer_pool, global_stats, user_stats from sys.all_tab_subpartitions
where table_owner=:owner and table_name=:table_name and partition_name=:partition_name
order by subpartition_position
