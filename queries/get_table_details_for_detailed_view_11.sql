select sys.all_tables.*, 
case when exists (select 0 from sys.all_external_tables where owner=sys.all_tables.owner and table_name=sys.all_tables.table_name) then 'YES'
else 'NO' end as is_external, NVL(fb.status, 'DISABLED') as flashback_status, fb.flashback_archive_name, fb.archive_table_name
from sys.all_tables left join sys.user_flashback_archive_tables fb
on sys.all_tables.owner=fb.owner_name and sys.all_tables.table_name=fb.table_name
where sys.all_tables.owner=:owner and sys.all_tables.table_name=:object_name