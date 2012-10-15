#ifndef USERCREATORGRANTSSIMPLELAYOUT_H
#define USERCREATORGRANTSSIMPLELAYOUT_H

#include <QWidget>

class QGroupBox;

class UserCreatorGrantsSimpleLayout : public QWidget
{
    Q_OBJECT
public:
    explicit UserCreatorGrantsSimpleLayout(QWidget *parent = 0);
    
private slots:
    void checkAllQuickRoles();
    void uncheckAllQuickRoles();
    void checkAllQuickPrivs();
    void uncheckAllQuickPrivs();

private:
    QWidget *createRolesBox();
    QWidget *createPrivilegesBox();

    QStringList quickRoleList;
    QStringList quickPrivList;

    QGroupBox *rolesBox;
    QGroupBox *privBox;
};

#endif // USERCREATORGRANTSSIMPLELAYOUT_H
