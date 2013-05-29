begin
   if :object_type = 'INDEX' then
      open :rs_out for select table_owner, table_name from sys.all_indexes where owner = :owner and index_name = :object_name;
   else --CONSTRAINT
      open :rs_out for select owner as table_owner, table_name from sys.all_constraints where owner = :owner and constraint_name = :object_name;
   end if;
end;
