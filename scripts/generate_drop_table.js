function generateDropTable(schemaName, tableName, cascade, purge)
{
   var ddl = 'DROP TABLE "'+schemaName+'"."'+tableName+'"';
   if(cascade){
      ddl = ddl.concat(' CASCADE CONSTRAINTS');
   }
   if(purge){
      ddl = ddl.concat(' PURGE');
   }
   
   return ddl;
}

function getScript()
{  
   return generateDropTable(schemaNameLabel.text, 
                             tableNameLabel.text, 
                             cascadeConstraintsCheckBox.checked,
                             purgeCheckBox.checked);
}
