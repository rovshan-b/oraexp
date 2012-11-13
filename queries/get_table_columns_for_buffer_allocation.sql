select column_name, data_type, char_used, char_length, data_scale from sys.all_tab_columns
where owner = :owner and table_name = :object_name order by column_id
