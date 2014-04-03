CREATE OR REPLACE PACKAGE department_pkg AUTHID DEFINER IS
 
  TYPE dept_info_record IS RECORD (
    dept_name  departments.department_name%TYPE,
    mgr_name   employees.last_name%TYPE,
    dept_size  PLS_INTEGER
  );
 
  -- Function declaration
 
  FUNCTION get_dept_info (dept_id NUMBER)
    RETURN dept_info_record
    RESULT_CACHE;
 
END department_pkg;
