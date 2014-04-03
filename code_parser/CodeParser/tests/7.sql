SELECT select_list 
    FROM table1 t_alias1 
    WHERE expr IN 
        (SELECT column_list 
            FROM table2 t_alias2 
            WHERE t_alias1.column 
               = t_alias2.column)
;
