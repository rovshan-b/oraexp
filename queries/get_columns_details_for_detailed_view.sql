select * from sys.all_tab_cols
where owner=:owner and table_name=:table_name
order by column_id