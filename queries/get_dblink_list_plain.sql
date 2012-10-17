select db_link as db_link_name from sys.all_db_links 
where (owner=:owner or owner='PUBLIC')
order by db_link
