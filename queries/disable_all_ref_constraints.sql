BEGIN
  FOR c IN
  (SELECT c.owner, c.table_name, c.constraint_name
   FROM sys.all_constraints c, sys.all_tables t
   WHERE c.table_name = t.table_name and t.owner='{owner}' and t.table_name in ({table_names})
   AND c.status = 'ENABLED' and c.constraint_type='R')
  LOOP
    dbms_utility.exec_ddl_statement('alter table "' || c.owner || '"."' || c.table_name || '" disable constraint ' || c.constraint_name);
  END LOOP;
END;
