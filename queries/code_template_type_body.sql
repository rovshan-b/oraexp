--body of data_typ1
CREATE OR REPLACE TYPE BODY data_typ1 IS   
    MEMBER FUNCTION prod (invent NUMBER) RETURN NUMBER IS 
    BEGIN 
       RETURN (year + invent);
    END; 
END; 

--body of demo typ2
CREATE OR REPLACE TYPE BODY demo_typ2 IS
   MEMBER FUNCTION get_square
   RETURN NUMBER
   IS x NUMBER;
   BEGIN
      SELECT c.col.a1*c.col.a1 INTO x
      FROM demo_tab2 c;
      RETURN (x);
   END;
END;
