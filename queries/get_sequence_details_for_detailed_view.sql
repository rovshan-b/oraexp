select o.created, o.last_ddl_time, 
s.* from sys.all_sequences s
inner join sys.all_objects o
on (s.sequence_owner=o.owner and s.sequence_name=o.object_name and o.object_type='SEQUENCE')
where s.sequence_owner=:owner and s.sequence_name=:object_name
