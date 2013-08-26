select object_name, owner, object_type, icon_name from (
select table_name as object_name, owner, 'TABLE' as object_type, 'table' as icon_name, (case when owner = :owner then 1 else 2 end) as sort_col1 from sys.all_all_tables where table_name like :object_name
escape '\'
union all
select object_name, owner, object_type, replace(lower(object_type),' ','_') as icon_name,
(case when owner = :owner then 1 else 2 end) as sort_col1
from sys.all_objects
where object_type in ('VIEW','MATERIALIZED VIEW','SEQUENCE','PACKAGE','PROCEDURE','FUNCTION','JAVA CLASS','SYNONYM') and object_name like :object_name
escape '\'
union all
select type_name as object_name, owner, 'TYPE' as object_type, 'type' as icon_name, 
(case when owner = :owner then 1 else 2 end) as sort_col1 from sys.all_types where typecode='OBJECT' and type_name like :object_name
escape '\'
order by sort_col1, object_name, owner
) t
