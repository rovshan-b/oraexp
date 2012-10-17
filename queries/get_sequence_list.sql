select sequence_name, min_value, max_value, cycle_flag, cache_size, last_number from sys.all_sequences
where sequence_owner=:owner order by sequence_name
