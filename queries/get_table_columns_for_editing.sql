select cols.column_name, data_type || 
case when char_used='C' and data_type not like 'N%' then ' ('||char_length||' CHAR)'
     when char_used='C' and data_type like 'N%' then ' ('||char_length||')'
     when char_used='B' and data_type not like 'N%' then ' ('||char_length||' BYTE)'
     when char_used='B' and data_type like 'N%' then ' ('||char_length||')'
     else ''
end ||
decode(data_type, 'NUMBER', 
     case when data_precision is not null and data_scale is null then ' ('||to_char(data_precision)||')'
          when data_precision is null and data_scale is not null then ' (*, '||to_char(data_scale)||')'
          when data_precision is not null and data_scale is not null then ' ('||to_char(data_precision)||', '||to_char(data_scale)||')'
     else '' end
     ,'')
as data_type, 
decode(nullable, 'Y', 'Yes', 'No') as nullable,
data_default,
virtual_column as virtual,
column_id,
sys.all_col_comments.comments,
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
            and const.constraint_type='U' 
          ) 
           then 1 
           else 0 
      end 
    ) as is_unique 
from sys.all_tab_cols cols left join sys.all_col_comments on 
    (cols.owner=sys.all_col_comments.owner and 
    cols.table_name=sys.all_col_comments.table_name and
    cols.column_name=sys.all_col_comments.column_name)
where cols.owner=:owner and cols.table_name=:table_name and hidden_column='NO'
order by column_id
