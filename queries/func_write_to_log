  PROCEDURE write_to_log(p_line varchar2) AS
     l_line VARCHAR2(255);
     l_done NUMBER;
     l_file utl_file.file_type;
  BEGIN
     l_file := utl_file.fopen('ORACLE_DIR', 'LOGFILE.TXT', 'A');
     utl_file.put_line(l_file, p_line||chr(13)||chr(10));
     utl_file.fflush(l_file);
     utl_file.fclose(l_file);
 END write_to_log;

