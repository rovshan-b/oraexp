#ifndef DATASELECTORDELEGATE_H
#define DATASELECTORDELEGATE_H

#include "lineeditwithbuttondelegate.h"

class IQueryScheduler;

class DataSelectorDelegate : public LineEditWithButtonDelegate
{
    Q_OBJECT
public:
    explicit DataSelectorDelegate(IQueryScheduler *queryScheduler, const QString &schema, const QString &objectName, QObject *parent);

    void setQueryScheduler(IQueryScheduler *queryScheduler);
    
protected slots:
    virtual void buttonClicked(LineEditWithButton *senderWidget);

private:
    IQueryScheduler *queryScheduler;
    QString schema;
    QString objectName;

    QString lastQuery;
    
};

#endif // DATASELECTORDELEGATE_H
