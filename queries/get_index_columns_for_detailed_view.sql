select c.*,
'user' as io_icon_column,
'index' as ind_icon_column,
'user' as to_icon_column,
'table' as tab_icon_column,
'column' as col_icon_column
from sys.all_ind_columns c
where c.index_owner=:owner and c.index_name=:object_name order by c.column_position
