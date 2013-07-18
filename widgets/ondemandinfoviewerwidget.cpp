#include "ondemandinfoviewerwidget.h"
#include <QTimer>

#include <iostream>
using namespace std;

OnDemandInfoViewerWidget::OnDemandInfoViewerWidget(QWidget *parent) :
    QWidget(parent), infoLoaded(false), infoLoading(false), needsRefresh(false)
{

}

void OnDemandInfoViewerWidget::loadInfo()
{
    if(infoLoaded==false && !infoLoading){
        doLoadInfo();
    }
}

bool OnDemandInfoViewerWidget::isInfoLoaded() const
{
    return infoLoaded;
}

void OnDemandInfoViewerWidget::refreshInfo()
{
    if(isLoading()){
        return;
    }

    clearInfo();
    doLoadInfo();
}

void OnDemandInfoViewerWidget::doLoadInfo()
{
    emit beforeLoadInfo();

    infoLoaded=true;
    infoLoading=true;
    needsRefresh = false;
}

void OnDemandInfoViewerWidget::clearInfo()
{
    infoLoaded=false;
}

bool OnDemandInfoViewerWidget::isLoading() const
{
    return this->infoLoading;
}

void OnDemandInfoViewerWidget::setLoadingComplete()
{
    infoLoading=false;

    emit afterLoadInfo();

    if(needsRefresh){
        needsRefresh = false;

        QTimer::singleShot(0, this, SLOT(refreshInfo()));
    }
}
