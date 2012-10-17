{@replace_with:func_clob_split}

function wrap_code(p_code clob)
return  clob
is
 v_cur_pos_nr number;
 v_next_pos_nr number;

 v_ddl_2a dbms_sql.varchar2a;
 v_wrapped_ddl_2a dbms_sql.varchar2a;

 v_wrapped_ddl_cl clob;

begin
 dbms_lob.createtemporary(v_wrapped_ddl_cl,true);

 v_ddl_2a := clob_split (p_code);
 
 -- wrap source code of the object
 v_wrapped_ddl_2a := dbms_ddl.wrap(v_ddl_2a,1,v_ddl_2a.count);

 -- Convert dbms_sql.varchar2a to CLOB
 for i in 1..v_wrapped_ddl_2a.count loop

   -- if it is the last line check the last character. If the last character is
   -- at the end of line, break down it to next line.
   if i = v_wrapped_ddl_2a.count then
     -- find the positions of line feeds (chr(10)) at the line
     v_cur_pos_nr := 0;
     v_next_pos_nr := 0;
     loop
       v_cur_pos_nr := instr(v_wrapped_ddl_2a(i), chr(10), v_cur_pos_nr+1);
       v_next_pos_nr := instr(v_wrapped_ddl_2a(i), chr(10), v_cur_pos_nr+1);

       exit when v_cur_pos_nr = 0 or v_next_pos_nr = (length(v_wrapped_ddl_2a(i))-1)
              or v_next_pos_nr = length(v_wrapped_ddl_2a(i));
     end loop;
     -- if the line length is 74, the last character is at the end of line.
     if (length(v_wrapped_ddl_2a(i)) - v_cur_pos_nr) = 74 then
       -- append the line as two lines; first one is until last character and
       -- second one is only last character
       dbms_lob.writeappend(v_wrapped_ddl_cl, v_cur_pos_nr, substr(v_wrapped_ddl_2a(i), 1, v_cur_pos_nr));
       dbms_lob.writeappend(v_wrapped_ddl_cl, 71, substr(v_wrapped_ddl_2a(i), v_cur_pos_nr+1, 71));
       dbms_lob.writeappend(v_wrapped_ddl_cl, 1, chr(10));
       dbms_lob.writeappend(v_wrapped_ddl_cl, 2, substr(v_wrapped_ddl_2a(i), v_cur_pos_nr+72, 1)||chr(10));
     else
       -- otherwise append the line directly
       dbms_lob.writeappend(v_wrapped_ddl_cl, length(v_wrapped_ddl_2a(i)),v_wrapped_ddl_2a(i));
     end if;
   else
     -- if it is not last line append the line directly
     dbms_lob.writeappend(v_wrapped_ddl_cl, length(v_wrapped_ddl_2a(i)),v_wrapped_ddl_2a(i));
   end if;

 end loop;
 -- add '/' character end of object wrap
 dbms_lob.writeappend(v_wrapped_ddl_cl,3,'/'||chr(10)||chr(10));

 -- return wrapped source code
 return v_wrapped_ddl_cl;
end;
