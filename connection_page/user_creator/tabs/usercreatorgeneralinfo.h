#ifndef USERCREATORGENERALINFO_H
#define USERCREATORGENERALINFO_H

#include "../usercreatortab.h"

class NameEditor;
class PasswordEditor;
class QComboBox;
class QLineEdit;
class TablespaceListComboBox;
class DbItemListComboBox;
class QCheckBox;
class QGroupBox;

class UserCreatorGeneralInfo : public UserCreatorTab
{
    Q_OBJECT
public:
    explicit UserCreatorGeneralInfo(const QString &objectName,
                                    UserCreatorTabs *userCreator,
                                    bool editMode,
                                    QWidget *parent = 0);

    virtual void setQueryScheduler(IQueryScheduler *queryScheduler);

private:
    QLayout *createForm(const QString &objectName);

    NameEditor *usernameEditor;
    QComboBox *identifiedByComboBox;
    PasswordEditor *passwordEditor;
    QLineEdit *dnEditor;
    TablespaceListComboBox *defaultTablespaceComboBox;
    TablespaceListComboBox *temporaryTablespaceComboBox;
    DbItemListComboBox *profileComboBox;
    QCheckBox *passwordExpiredCheckBox;
    QCheckBox *accountLockedCheckBox;
    QCheckBox *enableEditionsCheckBox;
};

#endif // USERCREATORGENERALINFO_H
