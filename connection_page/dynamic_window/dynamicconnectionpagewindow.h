#ifndef DYNAMICCONNECTIONPAGEWINDOW_H
#define DYNAMICCONNECTIONPAGEWINDOW_H

#include "../connectionpagewindow.h"
#include "beans/dynamicwindowinfo.h"
#include "util/scriptrunner.h"

class QFormLayout;
class CodeEditor;

class DynamicConnectionPageWindow : public ConnectionPageWindow
{
    Q_OBJECT
    Q_ENUMS(WidgetType)
public:
    enum WidgetType
    {
        Label,
        CheckBox
    };

    explicit DynamicConnectionPageWindow(QWidget *parent = 0);

    virtual void createUi();

    virtual void setConnection(DbConnection *db);

    void setWindowInfo(const DynamicWindowInfo &windowInfo);
    DynamicWindowInfo *getWindowInfo();

    void setActionProperties(const QHash<QString, QString> &properties);
    
private:
    DynamicWindowInfo windowInfo;
    QHash<QString, QString> actionProperties;

    CodeEditor *editor;

    void createForm(QFormLayout *form);
    QWidget *createDynamicWidget(WidgetType widgetType, const QHash<QString,QString> &attributes) const;

    QString getValue(const QString &attrValue) const;

    ScriptRunner scriptRunner;

    void registerScriptVariables();
    void updateQueryPane();
    
};

#endif // DYNAMICCONNECTIONPAGEWINDOW_H
