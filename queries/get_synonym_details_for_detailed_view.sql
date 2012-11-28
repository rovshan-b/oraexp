select o.created, o.last_ddl_time, 
s.* from sys.all_synonyms s
inner join sys.all_objects o
on (s.owner=o.owner and s.synonym_name=o.object_name and o.object_type='SYNONYM')
where s.owner=:owner and s.synonym_name=:object_name
