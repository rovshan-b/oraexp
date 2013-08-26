CREATE OR REPLACE PACKAGE BODY package_name AS

   --examples
   
   --function
   function get_emp_salary(p_emp_id in number) return number is
      --put declarations here
      l_salary number;
   begin
      select salary into l_salary from employees where emp_id=p_emp_id;
      return l_salary;
   exception when no_data_found then  --exception handling
      raise_application_error(-20001, 'Employee with provided ID does not exist');
   end;
   
   --procedure
   procedure get_emp_list(o_emp_cursor out t_cursor) is
   begin
      open o_emp_cursor for select * from employees where emp_name like 'A%';
   end;

END;
