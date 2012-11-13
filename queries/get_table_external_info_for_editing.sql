select et.*, trim(at.degree) as degree from sys.all_external_tables et
inner join sys.all_tables at on et.owner=at.owner and et.table_name=at.table_name
where et.owner=:owner and et.table_name=:object_name
