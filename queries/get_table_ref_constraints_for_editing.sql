select constraint_name, 
decode(constraint_type, 'P', 0, 'R', 1, 'U', 2, 'C', 3, 4) as cons_type, 
(
select col_names from (
SELECT constraint_name,
       LTRIM(MAX(SYS_CONNECT_BY_PATH(column_name,','))
       KEEP (DENSE_RANK LAST ORDER BY curr),',') AS col_names
FROM   (SELECT constraint_name,
               column_name,
               ROW_NUMBER() OVER (PARTITION BY constraint_name ORDER BY position) AS curr,
               ROW_NUMBER() OVER (PARTITION BY constraint_name ORDER BY position) -1 AS prev
        FROM   sys.all_cons_columns WHERE owner=:owner and table_name=:table_name)
GROUP BY constraint_name
CONNECT BY prev = PRIOR curr AND constraint_name = PRIOR constraint_name
START WITH curr = 1) t where constraint_name=ac.constraint_name
) as columns,
r_owner, 
(select table_name from sys.all_constraints where owner=ac.r_owner and constraint_name=ac.r_constraint_name) as r_table_name,
r_constraint_name, 
delete_rule, deferrable, deferred, generated
from sys.all_constraints ac
where owner=:owner and table_name=:table_name and constraint_type in ('P','R','U') 
order by decode(constraint_type, 'P', 0, 'R', 1, 'U', 2, 3), constraint_name
