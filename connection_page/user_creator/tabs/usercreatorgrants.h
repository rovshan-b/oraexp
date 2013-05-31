#ifndef USERCREATORGRANTS_H
#define USERCREATORGRANTS_H

#include "../usercreatortab.h"
#include "beans/privgrantinfo.h"
#include "beans/tablespacequotainfo.h"
#include "beans/objectgrantinfo.h"

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
                               DbObjectCreator::CreatorMode creatorMode,
                               QWidget *parent = 0);

    virtual void setQueryScheduler(IQueryScheduler *queryScheduler);

    void setUserInfo(UserInfo *userInfo);

    UserCreatorGrantsAdvancedLayout *getAdvancedLayout() const {return this->advancedLayout;}

    QList<PrivGrantInfo> getUserRoles() const;
    QList<PrivGrantInfo> getUserSysPrivs() const;
    QList<TablespaceQuotaInfo> getUserQuotas() const;
    QList<ObjectGrantInfo> getUserObjectPrivs() const;
    
    void removeIncorrectRows();

signals:
    void ddlChanged();

protected slots:
    void switchMode(bool simpleMode);
    
private slots:
    void syncSimpleLayout();
    void syncAdvancedLayout();

    void disableSync();
    void enableSync();

private:
    void createSimpleLayout();
    void createAdvancedLayout();

    void syncTable(QList<QCheckBox*> checkBoxes, DataTableAndToolBarWidget *table);
    void syncCheckBoxes(QList<QCheckBox*> checkBoxes, DataTableAndToolBarWidget *table);

    QRadioButton *simpleModeRadio;
    QRadioButton *advancedModeRadio;

    QStackedWidget *tab;
    UserCreatorGrantsSimpleLayout *simpleLayout;
    UserCreatorGrantsAdvancedLayout *advancedLayout;

    bool doDync;
    
};

#endif // USERCREATORGRANTS_H
