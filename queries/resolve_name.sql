declare
    OBJECT_INCOMPATIBLE_EX exception;
    pragma EXCEPTION_INIT(OBJECT_INCOMPATIBLE_EX, -4047);
    
    l_schema_name varchar2(50);
    l_part1 varchar2(100);
    l_part2 varchar2(100);
    l_db_link varchar2(150);
    l_part1_type number;
    l_object_number number;
    
    l_object_type varchar2(100);
begin
   for i in 1..9 loop
      begin
         DBMS_UTILITY.NAME_RESOLVE (
            :object_name, 
            i,
            l_schema_name, 
            l_part1, 
            l_part2,
            l_db_link, 
            l_part1_type, 
            l_object_number);
         exit;
       exception when OBJECT_INCOMPATIBLE_EX then
          null;
       end;
   end loop;
   
   select object_type into l_object_type from (
   select object_type from all_objects where object_id=l_object_number) t where rownum <= 1;
   
   if l_part1 is null and l_part2 is not null then
      l_part1 := l_part2;
      l_part2 := null;
   end if;
    
   open :rs_out for select l_object_type as object_type, l_schema_name as schema_name, l_part1 as part1, l_part2 as part2, l_db_link as dblink_name, l_object_number as object_number from dual;
end;
