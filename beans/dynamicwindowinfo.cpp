#include "dynamicwindowinfo.h"

DynamicWindowInfo::DynamicWindowInfo()
{
}

DynamicWindowInfo::~DynamicWindowInfo()
{
    qDeleteAll(widgetInfos);
}
