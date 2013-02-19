#include "queryexectask.h"


QueryExecTask::QueryExecTask() :
    retrieveScrollableResultsets(false),
    retrieveResultsetAsBindParameter(false),
    queryEndMonitor(0),
    isUserQuery(false)
{

}
