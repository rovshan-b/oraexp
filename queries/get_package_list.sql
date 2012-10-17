select o1.object_name as package_name, o1.status, o2.status as body_status from sys.all_objects o1 
left join sys.all_objects o2 on (o1.owner=o2.owner and o1.object_name=o2.object_name and o2.object_type='PACKAGE BODY')
where o1.object_type='PACKAGE' and o1.owner=:owner order by o1.object_name
