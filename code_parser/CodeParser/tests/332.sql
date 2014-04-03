create or replace type project_type as object
(
name varchar2(20),
stdate date,
member function GetAge return number,
member procedure change_stdate(newstdate date)
);
/ 
