#ifndef DYNAMICCONNECTIONPAGEWINDOW_H
#define DYNAMICCONNECTIONPAGEWINDOW_H

#include "../connectionpagewindow.h"
#include "beans/dynamicwindowinfo.h"
#include "scripting/scriptrunner.h"
#include "connectivity/queryresult.h"
#include "connectivity/sequentialqueryrunner.h"

class QFormLayout;
class CodeEditor;
class QDialogButtonBox;

class DynamicConnectionPageWindow : public ConnectionPageWindow
{
    Q_OBJECT
    Q_ENUMS(WidgetType)
public:
    enum WidgetType
    {
        Label,
        CheckBox,
        ComboBox,
        RadioButton
    };

    explicit DynamicConnectionPageWindow(DbUiManager *uiManager, QWidget *parent = 0);
    virtual ~DynamicConnectionPageWindow();

    virtual void createUi();

    virtual void setConnection(DbConnection *db);

    void setWindowInfo(DynamicWindowInfo *windowInfo);
    DynamicWindowInfo *getWindowInfo();

    void setActionProperties(const QHash<QString, QString> &properties);

private slots:
    void tabIndexChanged(int index);

    //void queryCompleted(const QueryResult &result);

    void beforeExecute(const QString &query, int startPos, int endPos);
    void queryResultAvailable(const QueryResult &result);
    void completed(bool success);

protected:
    virtual void setInProgress(bool inProgress);
    virtual void accept();
    
private:
    DynamicWindowInfo *windowInfo;
    QHash<QString, QString> actionProperties;

    QWidget *formWidget;
    CodeEditor *editor;
    QDialogButtonBox *buttonBox;

    void createForm(QFormLayout *form);
    QWidget *createDynamicWidget(WidgetType widgetType, DynamicWidgetInfo *widgetInfo) const;

    QString getValue(const QString &attrValue) const;

    ScriptRunner scriptRunner;

    SequentialQueryRunner sequentialRunner;

    void registerScriptVariables();
    void updateQueryPane();
    
};

#endif // DYNAMICCONNECTIONPAGEWINDOW_H
