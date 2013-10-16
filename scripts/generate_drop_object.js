{@replace_with:func_drop_object}

function getScript()
{  
   var schemaName = "";
   if(typeof schemaNameLabel !== "undefined"){
       schemaName = schemaNameLabel.text;
   }
   return generateDropObject(schemaName, 
                             objectNameLabel.text,
                             objectTypeLabel.text);
}
