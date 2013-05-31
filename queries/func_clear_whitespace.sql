{@replace_with:func_clob_replace}

procedure clear_whitespace(l_code in out clob) is
begin
   l_code := clob_replace(l_code, ' ', '');
   l_code := clob_replace(l_code, chr(10), '');
   l_code := clob_replace(l_code, chr(13), '');
end;
