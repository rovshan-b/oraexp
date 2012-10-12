#ifndef USERCREATORGENERALINFO_H
#define USERCREATORGENERALINFO_H

#include "../usercreatortab.h"

class NameEditor;
class PasswordEditor;
class QComboBox;
class QLineEdit;
class DbItemListComboBox;
class QCheckBox;

class UserCreatorGeneralInfo : public UserCreatorTab
{
    Q_OBJECT
public:
    explicit UserCreatorGeneralInfo(const QString &objectName,
                                    UserCreatorTabs *userCreator,
                                    bool editMode,
                                    QWidget *parent = 0);
    
private:
    QLayout *createForm();
    QWidget *createRolesBox();
    QWidget *createPrivilegesBox();

    NameEditor *usernameEditor;
    QComboBox *identifiedByComboBox;
    PasswordEditor *passwordEditor;
    QLineEdit *dnEditor;
    DbItemListComboBox *defaultTablespaceComboBox;
    DbItemListComboBox *temporaryTablespaceComboBox;
    DbItemListComboBox *profileComboBox;
    QCheckBox *passwordExpiredCheckBox;
    QCheckBox *accountLockedCheckBox;
    QCheckBox *enableEditionsCheckBox;

    QStringList roleList;
    QStringList privList;
};

#endif // USERCREATORGENERALINFO_H
