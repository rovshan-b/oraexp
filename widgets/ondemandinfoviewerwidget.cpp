#include "ondemandinfoviewerwidget.h"
#include <QMessageBox>

#include <iostream>
using namespace std;

OnDemandInfoViewerWidget::OnDemandInfoViewerWidget(QWidget *parent) :
    QWidget(parent), infoLoaded(false), infoLoading(false)
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
    clearInfo();
    doLoadInfo();
}

void OnDemandInfoViewerWidget::doLoadInfo()
{
    infoLoaded=true;
    infoLoading=true;
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
}
