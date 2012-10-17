select owner, table_name, temporary, duration, iot_type, 
case when exists (select 0 from sys.all_external_tables where owner=sys.all_tables.owner and table_name=sys.all_tables.table_name) then 'Y'
else 'N' end as is_external, partitioned
from sys.all_tables where (owner, table_name) in (

select owner, table_name from sys.all_constraints where (owner, constraint_name) in 
                (select const1.r_owner, const1.r_constraint_name from sys.all_constraints const1 
                inner join sys.all_cons_columns col1 on 
                (const1.table_name=col1.table_name and const1.constraint_name=col1.constraint_name 
                and const1.owner=col1.owner 
                and const1.owner=:owner and const1.table_name=:table_name 
                and col1.column_name=:column_name and const1.constraint_type='R'))
                
) order by owner, table_name
