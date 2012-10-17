select 
case when exists (select 0 from sys.all_views where view_name = 'USER_FLASHBACK_ARCHIVE_TABLES') then 'Y'
else 'N' end as flashback_archive_support
from dual
