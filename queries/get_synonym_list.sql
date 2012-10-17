select object_name, table_owner, table_name, db_link, status from sys.all_objects ao
inner join sys.all_synonyms s on 
(ao.object_type='SYNONYM' and ao.owner=s.owner and ao.object_name=s.synonym_name)
where ao.owner=:owner order by object_name
