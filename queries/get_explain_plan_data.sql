declare
   l_sql_id varchar2(50);
   l_child_number number;
begin
    
    if :autotrace = 0 then

        open :rs1 for SELECT id, level, operation, object_name, optimizer, options, cost, io_cost, cpu_cost, 
        CEIL(DECODE(COST,0,0,(COST-IO_COST)/COST)*100) percent_cpu,
        bytes, cardinality, other_tag, other, partition_start, partition_stop
        {@keep_if:>=10}
        , filter_predicates, access_predicates, time
        {}
          FROM PLAN_TABLE
        CONNECT BY prior id = parent_id
                AND prior statement_id = statement_id
        START WITH id = 0 AND statement_id = :statement_id
        ORDER BY id, level;
        
        open :rs2 for select * from table(dbms_xplan.display('PLAN_TABLE', :statement_id));
    
    else
       execute immediate 
        'select prev_sql_id, prev_child_number from v$session where sid = (select sid from v$mystat where rownum=1)'
        into l_sql_id, l_child_number;
    
        open :rs1 for 'select t.*, pa.pa.last_cr_buffer_gets from (
SELECT p.id, level, p.operation, p.object_name, p.optimizer, p.options, p.cost, p.io_cost, p.cpu_cost, 
        CEIL(DECODE(p.COST,0,0,(p.COST-p.IO_COST)/p.COST)*100) percent_cpu,
        p.bytes, p.cardinality, p.other_tag, p.other, p.partition_start, p.partition_stop
        --{@keep_if:>=10}
        , p.filter_predicates, p.access_predicates, p.time
        --{}
        , sql_id, child_number
  FROM V$SQL_PLAN p 
CONNECT BY prior p.id = p.parent_id
        AND prior p.sql_id = p.sql_id
        AND prior p.child_number = p.child_number
  START WITH id = 0
        AND p.sql_id = :sql_id and p.child_number = :child_number) t 
  LEFT JOIN V$SQL_PLAN_STATISTICS_ALL pa ON (t.sql_id=pa.sql_id and t.child_number=pa.child_number and t.id=pa.id)
  order by t.id' using l_sql_id, l_child_number;
        
        
        open :rs2 for 'select * from table(dbms_xplan.display_cursor(:sql_id, :child_number))' using l_sql_id, l_child_number;
    
    end if;
    
 end;
