select t.*,
(
select replace(rtrim (xmlagg (xmlelement (e, '"'||referenced_trigger_owner||'"."'|| referenced_trigger_name || '",')).extract ('//text()'), ','), '&quot;','"') ref_trg_names
from all_trigger_ordering where trigger_owner=t.owner and trigger_name=t.trigger_name and ordering_type='PRECEDES'
) as precedes,
(
select replace(rtrim (xmlagg (xmlelement (e, '"'||referenced_trigger_owner||'"."'|| referenced_trigger_name || '",')).extract ('//text()'), ','), '&quot;','"') ref_trg_names
from all_trigger_ordering where trigger_owner=t.owner and trigger_name=t.trigger_name and ordering_type='FOLLOWS'
) as follows
from sys.all_triggers t
where t.base_object_type='TABLE' and 
t.table_owner=:owner and t.table_name=:object_name
order by t.owner, t.trigger_name