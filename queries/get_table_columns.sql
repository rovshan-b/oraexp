select column_name, data_type, data_length, nullable, 
    ( 
      case when exists 
          (
            select 0 from sys.all_cons_columns conscols inner join sys.all_constraints const on 
            (conscols.owner=const.owner and conscols.table_name=const.table_name 
             and conscols.constraint_name=const.constraint_name 
            ) 
            where conscols.owner=cols.owner 
            and conscols.table_name=cols.table_name 
            and conscols.column_name=cols.column_name 
            and const.table_name=cols.table_name 
            and const.owner=cols.owner
            and const.constraint_type='P' 
          ) 
           then 1 
           else 0 
      end 
    ) as is_pk, 
    (
      case when exists 
          ( 
            select 0 from sys.all_cons_columns conscols inner join sys.all_constraints const on 
            (conscols.owner=const.owner and conscols.table_name=const.table_name 
             and conscols.constraint_name=const.constraint_name 
            ) 
            where conscols.owner=cols.owner 
           and conscols.table_name=cols.table_name 
           and conscols.column_name=cols.column_name 
            and const.table_name=cols.table_name 
            and const.owner=cols.owner
            and const.constraint_type='R' 
          ) 
          then 1 
          else 0 
      end 
    ) as is_fk, 
    ( 
      case when exists 
          ( 
            select 0 from sys.all_cons_columns conscols inner join sys.all_constraints const on 
            (conscols.owner=const.owner and conscols.table_name=const.table_name 
             and conscols.constraint_name=const.constraint_name 
            ) 
            where conscols.owner=cols.owner 
            and conscols.table_name=cols.table_name 
            and conscols.column_name=cols.column_name 
            and const.table_name=cols.table_name 
            and const.owner=cols.owner
            and const.constraint_type='C' 
          ) 
           then 1 
           else 0 
      end 
    ) as has_check_constraint, 
    ( 
      case when exists 
          ( 
            select 0 from sys.all_cons_columns conscols inner join sys.all_constraints const on 
            (conscols.owner=const.owner and conscols.table_name=const.table_name 
             and conscols.constraint_name=const.constraint_name 
            ) 
            where conscols.owner=cols.owner 
            and conscols.table_name=cols.table_name 
            and conscols.column_name=cols.column_name 
            and const.table_name=cols.table_name 
            and const.owner=cols.owner
            and const.constraint_type='U' 
          ) 
           then 1 
           else 0 
      end 
    ) as is_unique 
    from sys.all_tab_columns cols 
    where owner=:owner and table_name=:object_name 
    order by column_id 
