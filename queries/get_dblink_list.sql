select owner, db_link, username, host from sys.all_db_links 
where owner=:owner
order by db_link
