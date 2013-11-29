{@keep_if:>=10}
   exists (select 0 from gv$lock where inst_id = gv$session.inst_id and sid = gv$session.sid and (block=1 or request>0))
{}
{@keep_if:<10}
   exists (select 0 from v$lock where sid = v$session.sid and (block=1 or request>0))
{}
