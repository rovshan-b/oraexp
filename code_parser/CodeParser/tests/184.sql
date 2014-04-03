CREATE PROCEDURE find_root
   ( x IN REAL ) 
   IS LANGUAGE C
      NAME c_find_root
      LIBRARY c_utils
      PARAMETERS ( x BY REFERENCE );
