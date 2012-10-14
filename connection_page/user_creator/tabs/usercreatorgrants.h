#ifndef USERCREATORGRANTS_H
#define USERCREATORGRANTS_H

#include "../usercreatortab.h"

class QGroupBox;
class QStackedWidget;
class QRadioButton;

class UserCreatorGrants : public UserCreatorTab
{
    Q_OBJECT
public:
    explicit UserCreatorGrants(UserCreatorTabs *userCreator,
                               bool editMode,
                               QWidget *parent = 0);
    
private slots:
    void checkAllQuickRoles();
    void uncheckAllQuickRoles();
    void checkAllQuickPrivs();
    void uncheckAllQuickPrivs();

    void switchMode(bool simpleMode);
    
private:
    void createSimpleLayout();
    void createAdvancedLayout();

    QRadioButton *simpleModeRadio;
    QRadioButton *advancedModeRadio;

    QWidget *createRolesBox();
    QWidget *createPrivilegesBox();

    QStringList quickRoleList;
    QStringList quickPrivList;

    QGroupBox *rolesBox;
    QGroupBox *privBox;

    QStackedWidget *tab;
    
};

#endif // USERCREATORGRANTS_H
