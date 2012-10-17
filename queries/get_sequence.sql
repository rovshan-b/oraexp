select * from sys.all_sequences
where sequence_owner = :owner and sequence_name = :sequence_name
