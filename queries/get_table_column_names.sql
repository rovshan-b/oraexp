select column_name from sys.all_tab_columns cols where owner=:owner and table_name=:table_name order by column_id 
