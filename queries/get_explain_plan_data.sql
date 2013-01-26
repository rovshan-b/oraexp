SELECT id, level, operation, object_name, optimizer, options, cost, io_cost, cpu_cost, 
CEIL(DECODE(COST,0,0,(COST-IO_COST)/COST)*100) percent_cpu,
bytes, cardinality, other_tag, other, partition_start, partition_stop
{@keep_if:>=10}
, filter_predicates, access_predicates, time
{}
  FROM PLAN_TABLE
CONNECT BY prior id = parent_id
        AND prior statement_id = statement_id
START WITH id = 0 AND statement_id = :statement_id
ORDER BY id, level
