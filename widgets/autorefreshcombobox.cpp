#include "autorefreshcombobox.h"

AutoRefreshComboBox::AutoRefreshComboBox(QWidget *receiver) :
    QComboBox()
{
    for(int i=0; i<=120; i+=5){
        addItem(QString::number(i));
    }

    connect(&timer, SIGNAL(timeout()), receiver, SLOT(refreshInfo()));
    connect(this, SIGNAL(currentIndexChanged(QString)), this, SLOT(refreshIntervalChanged(QString)));
}

void AutoRefreshComboBox::refreshIntervalChanged(const QString &newInterval)
{
    if(timer.isActive()){
        timer.stop();
    }

    int interval = newInterval.toInt();

    if(interval > 0){
        timer.start(interval * 1000);
    }
}
