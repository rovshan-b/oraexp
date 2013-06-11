function generateDropTableObject(schemaName, tableName, objectType, objectName)
{
   var ddl = 'ALTER TABLE "'+schemaName+'"."'+tableName+'" DROP '+objectType+' "'+objectName+'"';  
   return ddl;
}
