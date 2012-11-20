select ao.*, s.* from sys.all_objects ao 
inner join sys.all_plsql_object_settings s on (ao.owner=s.owner and ao.object_name=s.name and ao.object_type=s.type)
where ao.owner=:owner and ao.object_name=:object_name
