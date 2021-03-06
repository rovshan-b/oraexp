select nvl(ai.prefix_length, -1) as prefix_length, 
ai.pct_threshold, 
(select column_name from sys.all_tab_columns cols where cols.owner=ai.owner and cols.table_name=ai.table_name 
 and cols.column_id=ai.include_column) as include_col_name,
(select table_name from sys.all_tables atab1 where 
 atab1.owner=ai.owner and atab1.iot_name=ai.table_name and atab1.iot_type='IOT_OVERFLOW') as overflow_table_name,
 (select logging from sys.all_tables atab2 where 
 atab2.owner=ai.owner and atab2.iot_name=ai.table_name and atab2.iot_type='IOT_OVERFLOW') as overflow_logging,
	case when exists (select 0 from sys.all_tables atab3 where atab3.owner=ai.owner and 
						 atab3.iot_name=ai.table_name and atab3.iot_type='IOT_MAPPING') then 1 
   else 0 end as has_mapping_table
from sys.all_indexes ai 
where (ai.owner, ai.index_name) in 
(select DECODE(index_owner, NULL, owner, index_owner), index_name from sys.all_constraints where owner=:owner and table_name=:object_name
and constraint_type='P')
and index_type='IOT - TOP'


/*select nvl(ai.prefix_length, -1) as prefix_length, 
ai.pct_threshold, ai.logging, 
cols.column_name as include_col_name,
atab1.table_name as overflow_table_name,
decode(atab3.table_name, null, 0, 1) as has_mapping_table
from sys.all_indexes ai 
left join sys.all_tab_columns cols on (cols.owner=ai.owner and cols.table_name=ai.table_name and cols.column_id=ai.include_column)
left join sys.all_tables atab1 on (atab1.owner=ai.owner and atab1.iot_name=ai.table_name and atab1.iot_type='IOT_OVERFLOW')
left join sys.all_tables atab3 on (atab3.owner=ai.owner and atab3.iot_name=ai.table_name and atab3.iot_type='IOT_MAPPING')
where (ai.owner, ai.index_name) in 
(select DECODE(index_owner, NULL, owner, index_owner), index_name from sys.all_constraints where owner=:owner and table_name=:object_name
and constraint_type='P')
and index_type='IOT - TOP'*/
