{@keep_if:>=10}
   select INST_ID, SID, SERIAL#, USERNAME, STATUS,  
   STATE, SECONDS_IN_WAIT,
   {@replace_with:v_session_command}, SERVER,
   MACHINE, OSUSER, 
   MODULE, ACTION, CLIENT_INFO, CLIENT_IDENTIFIER, 
   LOGON_TIME from gv$session where USERNAME is not null
{}
{@keep_if:<10}
   select SID, SERIAL#, USERNAME, STATUS, {@replace_with:v_session_command}, SERVER,
   MACHINE, OSUSER, 
   MODULE, ACTION, CLIENT_INFO, CLIENT_IDENTIFIER, 
   LOGON_TIME from v$session where USERNAME is not null
{}
