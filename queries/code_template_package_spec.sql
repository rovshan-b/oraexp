CREATE OR REPLACE PACKAGE package_name AS

   --examples

   --global variable
   g_var1 number;
   --cursor type
   type t_cursor is ref cursor;
   
   --record type
   type t_complex_type is record 
   (
       rec_id number,
       rec_name varchar2(50)
   );
   
   --function
   function get_emp_salary(p_emp_id in number) return number;
   
   --procedure
   procedure get_emp_list(o_emp_cursor out t_cursor);

END;
