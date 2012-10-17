select '"'||l.directory_owner||'"."'||l.directory_name||'"' as directory_name,
l.location from sys.all_external_locations l where l.owner=:owner and l.table_name=:table_name
order by l.directory_owner, l.directory_name, l.location
