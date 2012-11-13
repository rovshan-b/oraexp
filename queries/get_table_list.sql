select table_name, temporary, duration, iot_type, 
case when exists (select 0 from sys.all_external_tables where owner=sys.all_tables.owner and table_name=sys.all_tables.table_name) then 'Y'
else 'N' end as is_external, partitioned
from sys.all_tables where owner=:owner and (:object_name is null or table_name=:object_name) order by table_name
