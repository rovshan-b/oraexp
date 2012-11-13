select sys.all_lobs.segment_name, chunk, pctversion, retention, freepools, 
cache, logging, in_row,
s.tablespace_name, s.initial_extent, s.next_extent,
s.pct_increase, s.min_extents, s.max_extents,
s.freelists, s.freelist_groups, buffer_pool
from sys.all_lobs left join sys.user_segments s on (sys.all_lobs.segment_name=s.segment_name and s.segment_type='LOBSEGMENT')
where sys.all_lobs.owner=:owner and sys.all_lobs.table_name=:object_name
and sys.all_lobs.column_name=:column_name
