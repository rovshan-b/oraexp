function generateDropSchema(objectName, cascade)
{
   var ddl = 'DROP USER "'+objectName+'"';
   if(cascade){
      ddl = ddl.concat(' CASCADE');
   }
   
   return ddl;
}

function getScript()
{  
   return generateDropSchema(objectNameLabel.text, 
                            cascadeCheckBox.checked);
}
