function generateDropView(schemaName, viewName, cascade)
{
   var ddl = 'DROP VIEW "'+schemaName+'"."'+viewName+'"';
   if(cascade){
      ddl = ddl.concat(' CASCADE CONSTRAINTS');
   }
   
   return ddl;
}

function getScript()
{  
   return generateDropView(schemaNameLabel.text, 
                             viewNameLabel.text, 
                             cascadeConstraintsCheckBox.checked);
}
