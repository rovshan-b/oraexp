CREATE OR REPLACE TYPE BODY data_typ1 IS 
  MEMBER FUNCTION prod (invent NUMBER) RETURN NUMBER IS 
  BEGIN 
  RETURN (year + invent); 
  END; 
     MEMBER FUNCTION qtr(der_qtr DATE) RETURN CHAR IS 
     BEGIN 
       IF (der_qtr < TO_DATE('01-APR', 'DD-MON')) THEN 
         RETURN 'FIRST'; 
       ELSIF (der_qtr < TO_DATE('01-JUL', 'DD-MON')) THEN 
         RETURN 'SECOND'; 
       ELSIF (der_qtr < TO_DATE('01-OCT', 'DD-MON')) THEN 
         RETURN 'THIRD'; 
       ELSE 
         RETURN 'FOURTH'; 
       END IF; 
     END; 
   END;
/
