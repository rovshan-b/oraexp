select sys.all_tables.*, 
case when exists (select 0 from sys.all_external_tables where owner=sys.all_tables.owner and table_name=sys.all_tables.table_name) then 'YES'
else 'NO' end as is_external
from sys.all_tables where owner=:owner and table_name=:table_name
