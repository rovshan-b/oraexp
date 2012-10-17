select col_names from (
SELECT constraint_name,
       LTRIM(MAX(SYS_CONNECT_BY_PATH(column_name,','))
       KEEP (DENSE_RANK LAST ORDER BY curr),',') AS col_names
FROM   (SELECT constraint_name,
               column_name,
               ROW_NUMBER() OVER (PARTITION BY constraint_name ORDER BY position) AS curr,
               ROW_NUMBER() OVER (PARTITION BY constraint_name ORDER BY position) -1 AS prev
        FROM   sys.all_cons_columns WHERE owner=:owner and constraint_name=:constraint_name)
GROUP BY constraint_name
CONNECT BY prev = PRIOR curr AND constraint_name = PRIOR constraint_name
START WITH curr = 1) t
