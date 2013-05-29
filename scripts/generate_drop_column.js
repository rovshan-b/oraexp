function generateDropColumn(schemaName, tableName, columnName)
{
   var ddl = 'ALTER TABLE "'+schemaName+'"."'+tableName+'" DROP COLUMN "'+columnName+'"';  
   return ddl;
}

function getScript()
{  
   return generateDropColumn(schemaNameLabel.text, 
                             tableNameLabel.text, 
                             columnNameLabel.text);
}
