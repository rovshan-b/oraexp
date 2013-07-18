{@replace_with:func_kill_disconnect_sessions}

function getScript()
{  
   return getKillDisconnectScript(false, modeRadio.currentIndex == 1, modeRadio.currentIndex == 0);
}
