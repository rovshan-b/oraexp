{@replace_with:func_drop_object}

function getScript()
{  
   return generateDropObject(schemaNameLabel.text, 
                              objectNameLabel.text, 
                              'INDEX');
}
