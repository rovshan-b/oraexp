select * from sys.all_synonyms
where owner=:owner and synonym_name=:synonym_name
