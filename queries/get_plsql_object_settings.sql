{@keep_if:>=10}
select PLSQL_WARNINGS, PLSQL_CODE_TYPE from sys.all_plsql_object_settings
where owner=:owner and name=:object_name and type=:object_type
{}
{@keep_if:<10}
select 'DISABLE:ALL' as PLSQL_WARNINGS, 
DECODE(INSTR(param_value, 'INTERPRETED'), 0, 'NATIVE', 'INTERPRETED') as PLSQL_CODE_TYPE
from sys.all_stored_settings
where owner=:owner and object_name=:object_name and object_type=:object_type and param_name='plsql_compiler_flags'
{}
