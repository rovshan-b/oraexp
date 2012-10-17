select partition_name, high_value, tablespace_name, partition_position
from sys.all_ind_partitions 
where index_owner=:owner and index_name=:index_name
order by partition_position
