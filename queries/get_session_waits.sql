{@keep_if:>=10}
   select event, p1, p1text, p2, p2text, p3, p3text from gv$session_wait
   where inst_id = :inst_id and sid = :sid
   order by seq#
{}
{@keep_if:<10}
   select event, p1, p1text, p2, p2text, p3, p3text from v$session_wait
   where sid = :sid
   order by seq#
{}
