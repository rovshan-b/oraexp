CREATE OR REPLACE TRIGGER trigger_name
   BEFORE|AFTER|INSTEAD OF
   INSERT|DELETE|UPDATE --can be several, for example INSERT OR UPDATE
   OF column1, column2 --optional
   ON table_name
   FOR EACH ROW --optional
   WHEN (new.column1 > 20) --optional
DECLARE
   --put declarations here
BEGIN
   INSERT INTO log_table (column1, column2), VALUES (:new.column1, :old.column1);
END;
