#ifndef USERCREATORGRANTSSIMPLELAYOUT_H
#define USERCREATORGRANTSSIMPLELAYOUT_H

#include <QWidget>

class QGroupBox;
class QCheckBox;

class UserCreatorGrantsSimpleLayout : public QWidget
{
    Q_OBJECT
public:
    explicit UserCreatorGrantsSimpleLayout(QWidget *parent = 0);

    QList<QCheckBox*> getRoleCheckBoxes() const;
    QList<QCheckBox*> getPrivCheckBoxes() const;

signals:
    void ddlChanged();
    void beforeToggleAll();
    void afterToggleAll();
    
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
