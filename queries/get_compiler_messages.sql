select line, position, text, 
{@keep_if:>=10}attribute{}
{@keep_if:<10}'ERROR' as attribute{}
from sys.all_errors 
where owner=:owner and name=:object_name and type=:object_type order by sequence
