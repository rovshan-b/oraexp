select comments from sys.all_tab_comments where owner = :owner and table_name = :table_name and table_type = 'TABLE'
