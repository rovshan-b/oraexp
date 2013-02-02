begin
    
    if :autotrace = 0 then

        open :rs_out for SELECT id, level, operation, object_name, optimizer, options, cost, io_cost, cpu_cost, 
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
    
    else
    
        open :rs_out for 'SELECT id, level, operation, object_name, optimizer, options, cost, io_cost, cpu_cost, 
        CEIL(DECODE(COST,0,0,(COST-IO_COST)/COST)*100) percent_cpu,
        bytes, cardinality, other_tag, other, partition_start, partition_stop
        {@keep_if:>=10}
        , filter_predicates, access_predicates, time
        {}
        , last_cr_buffer_gets
          FROM 
          (select * from V$SQL_PLAN_STATISTICS_ALL
           where (sql_id, child_number) in
           (select SQL_ID, CHILD_NUMBER from v$sql 
            where sql_text like ''/*OraExp:AutoTrace:''||:statement_id||''*/%'')) t
        CONNECT BY prior id = parent_id
                AND prior sql_id = sql_id
        START WITH id = 0
        ORDER BY id, level' using :statement_id;
    
    end if;
    
 end;
