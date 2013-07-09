{@keep_if:>=10}
   select INST_ID, SID, SERIAL#, USERNAME, STATUS,  
   STATE, case when status='ACTIVE' then last_call_et else null end as seconds_in_wait,
   {@replace_with:v_session_command}, SERVER,
   MACHINE, OSUSER, 
   MODULE, ACTION, CLIENT_INFO, CLIENT_IDENTIFIER, 
   LOGON_TIME, 
   case when exists (select 0 from gv$mystat where inst_id = gv$session.inst_id and sid = gv$session.sid)  then 1 else 0 end as is_current 
   from gv$session where USERNAME is not null
{}
{@keep_if:<10}
   select SID, SERIAL#, USERNAME, STATUS, 
   case when status='ACTIVE' then last_call_et else null end as seconds_in_wait,
   {@replace_with:v_session_command}, SERVER,
   MACHINE, OSUSER, 
   MODULE, ACTION, CLIENT_INFO, CLIENT_IDENTIFIER, 
   LOGON_TIME, 
   case when exists (select 0 from v$mystat where sid = v$session.sid)  then 1 else 0 end as is_current 
   from v$session where USERNAME is not null
{}
