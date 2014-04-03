CREATE OR REPLACE PROCEDURE p (
  a        PLS_INTEGER,  -- IN by default
  b     IN PLS_INTEGER,
  c    OUT PLS_INTEGER,
  d IN OUT BINARY_FLOAT
) AUTHID DEFINER IS
BEGIN
  -- Print values of parameters:
 
  DBMS_OUTPUT.PUT_LINE('Inside procedure p:');
 
  DBMS_OUTPUT.PUT('IN a = ');
  DBMS_OUTPUT.PUT_LINE(NVL(TO_CHAR(a), 'NULL'));
 
  DBMS_OUTPUT.PUT('IN b = ');
  DBMS_OUTPUT.PUT_LINE(NVL(TO_CHAR(b), 'NULL'));
 
  DBMS_OUTPUT.PUT('OUT c = ');
  DBMS_OUTPUT.PUT_LINE(NVL(TO_CHAR(c), 'NULL'));
 
  DBMS_OUTPUT.PUT_LINE('IN OUT d = ' || TO_CHAR(d));
 
  -- Can reference IN parameters a and b,
  -- but cannot assign values to them.
 
  c := a+10;  -- Assign value to OUT parameter
  d := 10/b;  -- Assign value to IN OUT parameter
END;
