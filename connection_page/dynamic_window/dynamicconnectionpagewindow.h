#ifndef DYNAMICCONNECTIONPAGEWINDOW_H
#define DYNAMICCONNECTIONPAGEWINDOW_H

#include "../connectionpagewindow.h"
#include "beans/dynamicwindowinfo.h"

class QFormLayout;

class DynamicConnectionPageWindow : public ConnectionPageWindow
{
    Q_OBJECT
    Q_ENUMS(WidgetType)
public:
    enum WidgetType
    {
        Label
    };

    explicit DynamicConnectionPageWindow(QWidget *parent = 0);

    virtual void createUi();

    void setWindowInfo(const DynamicWindowInfo &windowInfo);
    DynamicWindowInfo *getWindowInfo();

    void setActionProperties(const QHash<QString, QString> &properties);
    
private:
    DynamicWindowInfo windowInfo;
    QHash<QString, QString> actionProperties;

    void createForm(QFormLayout *form);
    QWidget *createDynamicWidget(WidgetType widgetType, const QHash<QString,QString> &attributes) const;

    QString getValue(const QString &attrValue) const;
    
};

#endif // DYNAMICCONNECTIONPAGEWINDOW_H
