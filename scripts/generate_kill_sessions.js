function getScript()
{  
   var scripts=new Array();
   var sessions = g_sessionList.split(';');
   var line;
   
   for(i=0; i<sessions.length; ++i){
      line = "ALTER SYSTEM KILL SESSION '"+sessions[i]+"'";
      if(immediateCheckBox.checked){
         line += " IMMEDIATE";
      }
      line += ";";
      scripts.push(line);
   }
   
   return scripts;
}
