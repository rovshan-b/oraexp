{@replace_with:func_drop_table_object}

function getScript()
{  
   return generateDropTableObject(schemaNameLabel.text, 
                                   tableNameLabel.text, 
                                   'COLUMN',
                                   columnNameLabel.text);
}
