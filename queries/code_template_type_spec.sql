--varray type example
CREATE TYPE phone_list_typ_demo AS VARRAY(5) OF VARCHAR2(25);

--nested table type example
CREATE TYPE textdoc_typ AS OBJECT
    ( document_typ      VARCHAR2(32)
    , formatted_doc     BLOB
    ) ;

CREATE TYPE textdoc_tab AS TABLE OF textdoc_typ;

--nested table type containing varray
CREATE TYPE cust_address_typ2 AS OBJECT
       ( street_address     VARCHAR2(40)
       , postal_code        VARCHAR2(10)
       , city               VARCHAR2(30)
       , state_province     VARCHAR2(10)
       , country_id         CHAR(2)
       , phone              phone_list_typ_demo
       );

CREATE TYPE cust_nt_address_typ
   AS TABLE OF cust_address_typ2;
   


--constructor example
CREATE TYPE demo_typ1 AS OBJECT (a1 NUMBER, a2 NUMBER);

CREATE TABLE demo_tab1 (b1 NUMBER, b2 demo_typ1);

INSERT INTO demo_tab1 VALUES (1, demo_typ1(2,3));

--object type example
CREATE TYPE customer_typ_demo AS OBJECT
    ( customer_id        NUMBER(6)
    , cust_first_name    VARCHAR2(20)
    , cust_last_name     VARCHAR2(20)
    , cust_address       CUST_ADDRESS_TYP
    , phone_numbers      PHONE_LIST_TYP
    , nls_language       VARCHAR2(3)
    , nls_territory      VARCHAR2(30)
    , credit_limit       NUMBER(9,2)
    , cust_email         VARCHAR2(30)
    , cust_orders        ORDER_LIST_TYP
    ) ;
    
--type with member function (implemented in type body)
CREATE TYPE data_typ1 AS OBJECT 
   ( year NUMBER, 
     MEMBER FUNCTION prod(invent NUMBER) RETURN NUMBER 
   ); 
   
--subtype example
CREATE TYPE corporate_customer_typ_demo UNDER customer_typ_demo
    ( account_mgr_id     NUMBER(6)
    );
    

--calling member method example
CREATE TYPE demo_typ2 AS OBJECT (a1 NUMBER,  
   MEMBER FUNCTION get_square RETURN NUMBER);  --member implemented in type body

CREATE TABLE demo_tab2(col demo_typ2); 

INSERT INTO demo_tab2 VALUES (demo_typ2(2));

SELECT t.col.get_square() FROM demo_tab2 t;
