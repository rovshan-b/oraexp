function generateDropObject(schemaName, objectName, objectType)
{
   var ddl = 'DROP '+objectType;  
   if(schemaName != ''){
      ddl = ddl.concat(' "'+schemaName+'"."'+objectName+'"');
   }else{
      ddl = ddl.concat(' "'+objectName+'"');
   }
   return ddl;
}
