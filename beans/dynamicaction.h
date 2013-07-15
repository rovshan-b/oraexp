#ifndef DYNAMICACTION_H
#define DYNAMICACTION_H

#include <QAction>

class DbUiManager;

class DynamicAction : public QAction
{
    Q_OBJECT
public:
    explicit DynamicAction(const QIcon &icon,
                           const QString &text,
                           const QObject *receiver=0,
                           const char *slotName=0,
                           QObject *parent = 0);

    virtual ~DynamicAction() {}
    
    QHash<QString,QString> properties;
    
    void execute(DbUiManager *uiManager);

protected:
    virtual void setProperties();
};

#endif // DYNAMICACTION_H
