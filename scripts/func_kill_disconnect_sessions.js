function getKillDisconnectScript(kill, immediate, post_transaction)
{  
   var scripts=new Array();
   var sessions = g_sessionList.split(';');
   var line;
   
   for(i=0; i<sessions.length; ++i){
      line = "ALTER SYSTEM ";
      line += kill ? "KILL" : "DISCONNECT";
      line += " SESSION '"+sessions[i]+"'";
      if(post_transaction){
         line += " POST_TRANSACTION";
      }
      if(immediate){
         line += " IMMEDIATE";
      }
      line += ";";
      scripts.push(line);
   }
   
   return scripts;
}
