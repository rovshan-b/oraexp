{@keep_if:>=10}
   select opname, target, target_desc, sofar, totalwork, elapsed_seconds, 
   round(sofar/totalwork*100,2)||'%' complete, time_remaining, message
   from   gv$session_longops
   where totalwork != 0
   and sofar != totalwork
   and inst_id = :inst_id and sid = :sid
   order by opname
{}
{@keep_if:<10}
   select opname, target, target_desc, sofar, totalwork, elapsed_seconds, 
   round(sofar/totalwork*100,2)||'%' complete, time_remaining, message
   from   v$session_longops
   where totalwork != 0
   and sofar != totalwork
   and sid = :sid
   order by opname
{}
