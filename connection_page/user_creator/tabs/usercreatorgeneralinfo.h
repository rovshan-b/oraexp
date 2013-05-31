#ifndef USERCREATORGENERALINFO_H
#define USERCREATORGENERALINFO_H

#include "../usercreatortab.h"
#include "beans/usergeneralinfo.h"

class NameEditor;
class PasswordEditor;
class QComboBox;
class QLineEdit;
class TablespaceListComboBox;
class DbItemListComboBox;
class QCheckBox;
class QGroupBox;
class UserInfo;

class UserCreatorGeneralInfo : public UserCreatorTab
{
    Q_OBJECT
public:
    explicit UserCreatorGeneralInfo(const QString &objectName,
                                    UserCreatorTabs *objectCreator,
                                    DbObjectCreator::CreatorMode creatorMode,
                                    QWidget *parent = 0);

    virtual void setQueryScheduler(IQueryScheduler *queryScheduler);

    void setUserInfo(UserInfo *userInfo);
    UserGeneralInfo getUserGeneralInfo() const;

    QString getUserName() const;

signals:
    void ddlChanged();

private slots:
    void enableControls();

    void alterQuerySucceeded(const QString &taskName);

private:
    QLayout *createForm(const QString &objectName);
    void disableControlsForEditMode();

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
