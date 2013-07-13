{@keep_if:>=10}
   SELECT s1.username || '@' || s1.machine
   || ' ( SID=' || s1.sid || ' )  is blocking '
   || s2.username || '@' || s2.machine || ' ( SID=' || s2.sid || ' ) ' AS locks
   FROM gv$lock l1, gv$session s1, gv$lock l2, gv$session s2
   WHERE (s1.sid=l1.sid AND s1.inst_id=l1.inst_id AND s2.sid=l2.sid AND s2.inst_id=l2.inst_id)
   AND l1.BLOCK=1 AND l2.request > 0
   AND l1.id1 = l2.id1 AND l1.inst_id = l2.inst_id
   AND l2.id2 = l2.id2
   AND 
   ((s1.sid = :sid and s1.inst_id = :inst_id) or (s2.sid = :sid and s2.inst_id = :inst_id))
{}
{@keep_if:<10}
   SELECT s1.username || '@' || s1.machine
    || ' ( SID=' || s1.sid || ' )  is blocking '
    || s2.username || '@' || s2.machine || ' ( SID=' || s2.sid || ' ) ' AS locks
    FROM v$lock l1, v$session s1, v$lock l2, v$session s2
    WHERE s1.sid=l1.sid AND s2.sid=l2.sid
    AND l1.BLOCK=1 AND l2.request > 0
    AND l1.id1 = l2.id1
    AND l2.id2 = l2.id2
    AND 
    (s1.sid = :sid or s2.sid = :sid)
{}
