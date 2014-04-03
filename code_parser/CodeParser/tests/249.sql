DECLARE
  aa  CONSTANT PLS_INTEGER := 1;
  bb  PLS_INTEGER  := 2;
  cc  PLS_INTEGER  := 3;
  dd  BINARY_FLOAT := 4;
  ee  PLS_INTEGER;
  ff  BINARY_FLOAT := 5;
BEGIN
  DBMS_OUTPUT.PUT_LINE('Before invoking procedure p:');
 
  DBMS_OUTPUT.PUT('aa = ');
  DBMS_OUTPUT.PUT_LINE(NVL(TO_CHAR(aa), 'NULL'));
 
  DBMS_OUTPUT.PUT('bb = ');
  DBMS_OUTPUT.PUT_LINE(NVL(TO_CHAR(bb), 'NULL'));
 
  DBMS_OUTPUT.PUT('cc = ');
  DBMS_OUTPUT.PUT_LINE(NVL(TO_CHAR(cc), 'NULL'));
 
  DBMS_OUTPUT.PUT_LINE('dd = ' || TO_CHAR(dd));
 
  p (aa, -- constant
     bb, -- initialized variable
     cc, -- initialized variable 
     dd  -- initialized variable
  );
 
  DBMS_OUTPUT.PUT_LINE('After invoking procedure p:');
 
  DBMS_OUTPUT.PUT('aa = ');
  DBMS_OUTPUT.PUT_LINE(NVL(TO_CHAR(aa), 'NULL'));
 
  DBMS_OUTPUT.PUT('bb = ');
  DBMS_OUTPUT.PUT_LINE(NVL(TO_CHAR(bb), 'NULL'));
 
  DBMS_OUTPUT.PUT('cc = ');
  DBMS_OUTPUT.PUT_LINE(NVL(TO_CHAR(cc), 'NULL'));
 
  DBMS_OUTPUT.PUT_LINE('dd = ' || TO_CHAR(dd));
 
  DBMS_OUTPUT.PUT_LINE('Before invoking procedure p:');
 
  DBMS_OUTPUT.PUT('ee = ');
  DBMS_OUTPUT.PUT_LINE(NVL(TO_CHAR(ee), 'NULL'));
 
  DBMS_OUTPUT.PUT_LINE('ff = ' || TO_CHAR(ff));
 
  p (1,        -- literal 
     (bb+3)*4, -- expression 
     ee,       -- uninitialized variable 
     ff        -- initialized variable
   );
 
  DBMS_OUTPUT.PUT_LINE('After invoking procedure p:');
 
  DBMS_OUTPUT.PUT('ee = ');
  DBMS_OUTPUT.PUT_LINE(NVL(TO_CHAR(ee), 'NULL'));
 
  DBMS_OUTPUT.PUT_LINE('ff = ' || TO_CHAR(ff));
END;
