select table_name, column_name from sys.all_cons_columns where owner=:owner and constraint_name=:constraint_name order by position
