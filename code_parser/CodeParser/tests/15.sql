SELECT * FROM employees e, (SELECT * FROM departments d
                            WHERE e.department_id = d.department_id)
;
