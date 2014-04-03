CREATE OR REPLACE FUNCTION factorial (
  n POSITIVE
) RETURN POSITIVE
  AUTHID DEFINER
IS
BEGIN
  IF n = 1 THEN                 -- terminating condition
    RETURN n;
  ELSE
    RETURN n * factorial(n-1);  -- recursive invocation
  END IF;
END;
