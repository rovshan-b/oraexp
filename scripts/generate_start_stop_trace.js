function getScript()
{  
   var scripts=new Array();
   var sessions = g_sessionList.split(';');
   var line;
   var sessionParts;
   
   var start = (actionRadio.currentIndex == 0);
   
   for(i=0; i<sessions.length; ++i){
      sessionParts = sessions[i].split(",");
      if(sessionParts.length < 2){
         continue;
      }
      line = "BEGIN SYS.DBMS_SYSTEM.SET_SQL_TRACE_IN_SESSION("+sessionParts[0]+", "+sessionParts[1]+", "+(start ? "TRUE" : "FALSE")+"); END;\n/"
      scripts.push(line);
   }
   
   return scripts;
}
