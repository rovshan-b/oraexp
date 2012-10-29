#ifndef USERCREATORTABS_H
#define USERCREATORTABS_H

#include "widgets/subtabwidget.h"
#include "connectivity/ociexception.h"
#include "beans/querylistitem.h"

class IQueryScheduler;
class UserCreatorGeneralInfo;
class UserCreatorGrants;
class DbObjectInfo;
class MetadataLoader;
class UserInfo;

class UserCreatorTabs : public SubTabWidget
{
    Q_OBJECT
public:
    explicit UserCreatorTabs(const QString &objectName, QWidget *parent = 0);
    
    void setQueryScheduler(IQueryScheduler *queryScheduler);

    virtual QString generateCreateDdl();
    virtual QList< QueryListItem > generateAlterDdl();

    UserInfo *getOriginalUserInfo() const {return this->originalUserInfo;}

    bool beforeCreate() const;
    bool beforeAlter() const;

    QString getUserName() const;

signals:
    void objectInfoLoaded();
    void ddlChanged();

private slots:
    void userInfoReady(DbObjectInfo *userInfo, MetadataLoader *loader);
    void loadError(const QString &taskName, const OciException &ex, MetadataLoader *loader);

private:
    QString objectName;
    UserInfo *originalUserInfo;
    bool editMode;
    IQueryScheduler *queryScheduler;

    UserCreatorGeneralInfo *generalInfoTab;
    UserCreatorGrants *grantsTab;

    UserInfo getUserInfo() const;
};

#endif // USERCREATORTABS_H
