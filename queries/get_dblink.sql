declare
   l_has_access number;
begin
   select case when exists (select 0 from sys.all_tables where owner='SYS' and table_name='LINK$') then 1 else 0 end into l_has_access from dual;

   if l_has_access = 1 then
       open :rs_out for 'select nvl(u.username, ''PUBLIC'') as owner, name as db_link, userid as username, password, host, authusr, 
       authpwd, 1 as is_complete from sys.link$ l 
       left join sys.all_users u on l.owner#=u.user_id 
       where (u.username = :owner or (:owner=''PUBLIC'' and l.owner#=1)) and l.name = :dblink_name' using :owner, :owner, :dblink_name;
   else
       if :owner = 'PUBLIC' then
           open :rs_out for select owner, db_link, username, null as password, host, null as authusr, null as authpwd, 0 as is_complete from sys.all_db_links
           where owner = :owner and db_link = :dblink_name;
       else
           open :rs_out for select owner_name as owner, name as db_link, userid as username, password, host, authusr, authpwd, 1 as is_complete from SYS.KU$_DBLINK_VIEW	
           where owner_name = :owner and name = :dblink_name;
       end if;
   end if;
end;
