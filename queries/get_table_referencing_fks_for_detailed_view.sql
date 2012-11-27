select owner, table_name, constraint_name, null as columns,
'user' as owner_icon_column,
'table' as tab_icon_column, 
'constraint_fk' as fk_icon_column
from sys.all_constraints 
where r_owner=:owner and constraint_type='R' and
r_constraint_name in (select constraint_name from sys.all_constraints 
 where owner=:owner and table_name=:object_name and constraint_type='P') 
order by constraint_name
