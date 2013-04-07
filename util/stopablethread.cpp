#include "stopablethread.h"

StopableThread::StopableThread(QObject *parent) :
    QThread(parent), stopped(false)
{
}
