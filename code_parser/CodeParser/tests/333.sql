create or replace type body project_type is
member function GetAge return number is
begin
return sysdate - stdate;
end;
member procedure change_stdate(newstdate date)
is
begin
-- make sure new date is not after sysdate
if newstdate > sysdate then
raise_application_error(-20011, 'Inval
id starting date');
else
stdate := newstdate;
end if;
end;
end;
/ 
