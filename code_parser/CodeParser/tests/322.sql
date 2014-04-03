CREATE TYPE textdoc_typ AS OBJECT
    ( document_typ      VARCHAR2(32)
    , formatted_doc     BLOB
    ) ;
    
/

CREATE TYPE textdoc_tab AS TABLE OF textdoc_typ;
