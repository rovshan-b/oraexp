select partition_name, high_value, tablespace_name, partition_position
from sys.all_tab_partitions 
where table_owner=:owner and table_name=:table_name
order by partition_position
