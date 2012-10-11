#ifndef USERCREATORGENERALINFO_H
#define USERCREATORGENERALINFO_H

#include "../usercreatortab.h"

class UserCreatorGeneralInfo : public UserCreatorTab
{
    Q_OBJECT
public:
    explicit UserCreatorGeneralInfo(const QString &objectName,
                                    UserCreatorTabs *userCreator,
                                    bool editMode,
                                    QWidget *parent = 0);
    
signals:
    
public slots:
    
};

#endif // USERCREATORGENERALINFO_H
