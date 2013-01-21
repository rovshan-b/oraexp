SELECT id, level, operation, object_name, optimizer, options, cost, io_cost, cpu_cost, 
bytes, cardinality, other, partition_id
{@keep_if:>=10}
, filter_predicates, access_predicates
{}
  FROM PLAN_TABLE
CONNECT BY prior id = parent_id
        AND prior statement_id = statement_id
START WITH id = 0 AND statement_id = :statement_id
ORDER BY id, level
