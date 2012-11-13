select object_name, object_type, icon_name from (
select table_name as object_name, 'TABLE' as object_type, 'table' as icon_name, 1 as sort_col from sys.all_all_tables where owner=:owner
union all
select object_name, object_type, replace(lower(object_type),' ','_') as icon_name,
case object_type 
  when 'TABLE' then 1
  when 'VIEW' then 2
  when 'MATERIALIZED VIEW' then 3
  when 'PACKAGE' then 4
  when 'PROCEDURE' then 5
  when 'FUNCTION' then 6
  when 'SEQUENCE' then 7
  when 'TYPE' then 8
  when 'SYNONYM' then 9
  when 'JAVA_CLASS' then 10
  else 11
end as sort_col
from sys.all_objects
where object_type in ('VIEW','MATERIALIZED VIEW','SEQUENCE','PACKAGE','PROCEDURE','FUNCTION','JAVA CLASS','SYNONYM') and owner=:owner
union all
select type_name as object_name, 'TYPE' as object_type, 'type' as icon_name, 8 as sort_col from sys.all_types where typecode='OBJECT' and owner=:owner
union all
select object_name, object_type, replace(lower(object_type),' ','_') as icon_name, 11 as sort_col from sys.all_objects
where object_type not in ('TABLE', 'VIEW','MATERIALIZED VIEW','SEQUENCE','PACKAGE', 'PACKAGE BODY','PROCEDURE','FUNCTION','JAVA CLASS','SYNONYM','TYPE','TYPE BODY','TRIGGER') and owner=:owner
order by sort_col, object_name
) t