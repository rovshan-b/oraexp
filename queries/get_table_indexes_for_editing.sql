select ai.*, pi.locality
from sys.all_indexes ai left join sys.all_part_indexes pi
on (ai.owner=pi.owner and ai.index_name=pi.index_name and ai.table_name=pi.table_name)
where ai.table_owner=:owner and ai.table_name=:table_name and ai.table_type='TABLE'
and ai.generated='N'
and not exists (select 0 from sys.all_constraints ac where owner=ai.owner and table_name=ai.table_name and index_name=ai.index_name)
order by ai.index_name
