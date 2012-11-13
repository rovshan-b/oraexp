select subpartition_name, subpartition_position as partition_position, tablespace_name, high_bound as high_value from sys.all_subpartition_templates
where user_name=:owner and table_name=:object_name
order by subpartition_position
