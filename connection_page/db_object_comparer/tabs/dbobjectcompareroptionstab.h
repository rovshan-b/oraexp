#ifndef DBOBJECTCOMPAREROPTIONSTAB_H
#define DBOBJECTCOMPAREROPTIONSTAB_H

#include <QWidget>
#include "beans/dbobjectcomparisonoptions.h"
class DbConnection;

class DbObjectComparerOptionsTab : public QWidget
{
    Q_OBJECT
public:
    explicit DbObjectComparerOptionsTab(QWidget *parent = 0);

    virtual void createUi(){}

    virtual void targetConnectionEstablished(DbConnection *sourceDb, DbConnection *targetDb);

    virtual DbObjectComparisonOptions *getOptions() const = 0;
    
signals:
    
public slots:
    
};

#endif // DBOBJECTCOMPAREROPTIONSTAB_H
