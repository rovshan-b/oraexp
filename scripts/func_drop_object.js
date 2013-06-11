function generateDropObject(schemaName, objectName, objectType)
{
   var ddl = 'DROP '+objectType+' "'+schemaName+'"."'+objectName+'"';  
   return ddl;
}
