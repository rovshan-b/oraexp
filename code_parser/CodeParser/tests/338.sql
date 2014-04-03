DECLARE
 myssn ssn;
BEGIN
  myssn := ssn(ssn_in=>'232-22-5678');
  INSERT INTO person VALUES ('Morgan', myssn);

  myssn := ssn(ssn_in=>'444=55-6789');
  INSERT INTO person VALUES ('Morgan', myssn);

  myssn := ssn(ssn_in=>'123-45-6789');
  INSERT INTO person VALUES ('Cline', myssn);
  COMMIT;
END;
/

DECLARE
  myssn ssn;
BEGIN
   myssn := ssn(ssn_in=>'232-22-5678');
   INSERT INTO person VALUES ('Morgan', myssn);
/*
  myssn := ssn(ssn_in=>'444=55-6789');
  INSERT INTO person VALUES ('Morgan', myssn);
*/
   myssn := ssn(ssn_in=>'123-45-6789');
   INSERT INTO person VALUES ('Cline', myssn);
   COMMIT;
END;
/
