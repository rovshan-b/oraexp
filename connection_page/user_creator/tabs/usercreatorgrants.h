#ifndef USERCREATORGRANTS_H
#define USERCREATORGRANTS_H

#include "../usercreatortab.h"
#include "beans/privgrantinfo.h"

class QGroupBox;
class QStackedWidget;
class QRadioButton;
class QCheckBox;
class DataTableAndToolBarWidget;
class UserCreatorGrantsSimpleLayout;
class UserCreatorGrantsAdvancedLayout;

class UserCreatorGrants : public UserCreatorTab
{
    Q_OBJECT
public:
    explicit UserCreatorGrants(UserCreatorTabs *objectCreator,
                               bool editMode,
                               QWidget *parent = 0);

    virtual void setQueryScheduler(IQueryScheduler *queryScheduler);

    void setUserInfo(UserInfo *userInfo);

    QList<PrivGrantInfo> getUserRoles() const;
    QList<PrivGrantInfo> getUserSysPrivs() const;
    
    void removeIncorrectRows();

signals:
    void ddlChanged();

protected slots:
    void switchMode(bool simpleMode);
    void alterQuerySucceeded(const QString &taskName);
    void alterQueryError(const QString &taskName);
    
private:
    void createSimpleLayout();
    void createAdvancedLayout();

    void syncSimpleLayout();
    void syncAdvancedLayout();

    void syncTable(QList<QCheckBox*> checkBoxes, DataTableAndToolBarWidget *table);
    void syncCheckBoxes(QList<QCheckBox*> checkBoxes, DataTableAndToolBarWidget *table);

    QRadioButton *simpleModeRadio;
    QRadioButton *advancedModeRadio;

    QStackedWidget *tab;
    UserCreatorGrantsSimpleLayout *simpleLayout;
    UserCreatorGrantsAdvancedLayout *advancedLayout;
    
};

#endif // USERCREATORGRANTS_H
