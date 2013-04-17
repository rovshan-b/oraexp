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
     else '' end,
     'RAW', '('||data_length||')',
     '')
as data_type,
decode(nullable, 'Y', 'Yes', 'No') as nullable,
data_default,
column_id,
sys.all_col_comments.comments,
uc.updatable, uc.insertable, uc.deletable
from sys.all_tab_cols cols left join sys.all_col_comments on 
(cols.owner=sys.all_col_comments.owner and 
cols.table_name=sys.all_col_comments.table_name and
cols.column_name=sys.all_col_comments.column_name)
inner join sys.all_updatable_columns uc on
(cols.owner=uc.owner and cols.table_name=uc.table_name and cols.column_name=uc.column_name)
where cols.owner=:owner and cols.table_name=:object_name and cols.hidden_column='NO'
order by column_id
