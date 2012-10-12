#ifndef USERCREATORTABS_H
#define USERCREATORTABS_H

#include "widgets/subtabwidget.h"

class IQueryScheduler;
class UserCreatorGeneralInfo;

class UserCreatorTabs : public SubTabWidget
{
    Q_OBJECT
public:
    explicit UserCreatorTabs(const QString &objectName, QWidget *parent = 0);
    
    void setQueryScheduler(IQueryScheduler *queryScheduler);

signals:
    void objectInfoLoaded();
    void ddlChanged();

private:
    bool editMode;

    UserCreatorGeneralInfo *generalInfoTab;
    
};

#endif // USERCREATORTABS_H
