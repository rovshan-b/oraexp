select constraint_name, constraint_type, search_condition, delete_rule, status from sys.all_constraints 
where owner=:owner and table_name=:table_name order by decode(constraint_type, 'P', 0, 'R', 1, 'U', 2, 3),
constraint_name
