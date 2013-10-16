select table_name, temporary, duration, iot_type, 
case when exists (select 0 from sys.all_external_tables where owner=sys.all_tables.owner and table_name=sys.all_tables.table_name) then 'Y'
else 'N' end as is_external, partitioned
from sys.all_tables where 
owner=:owner and (:object_name is null or table_name=:object_name) 
and (owner, table_name) not in (select owner, mview_name from all_mviews 
                                 union all 
                                 select log_owner, log_table from all_mview_logs)
order by table_name
