select column_name, data_type, char_used, decode(char_length, 0, data_length, char_length) as char_length, data_scale from sys.all_tab_columns
where owner = :owner and table_name = :object_name order by column_id
