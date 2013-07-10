{@keep_if:>=10}
   select sql_text from gv$sqltext_with_newlines
   where inst_id = :inst_id and sql_id = (
      select sql_id from gv$session where inst_id = :inst_id and sid = :sid)
   order by piece
{}
{@keep_if:<10}
   select sql_text from v$sqltext_with_newlines
   where sql_id = (
      select sql_id from v$session where sid = :sid)
   order by piece
{}
