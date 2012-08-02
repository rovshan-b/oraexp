#ifndef TABLEINFOVIEWERTABS_H
#define TABLEINFOVIEWERTABS_H

#include "widgets/subtabwidget.h"
#include "interfaces/iqueryscheduler.h"

class DbConnection;

class TableInfoViewerTabs : public SubTabWidget
{
    Q_OBJECT
public:
    explicit TableInfoViewerTabs(const QString &schemaName,
                                 const QString &tableName,
                                 QWidget *parent);

    void setConnection(DbConnection *);

    IQueryScheduler *scheduler() const {return this->queryScheduler;}

signals:

public slots:
    void loadTabData(int index);

private:
    bool connectionAvailable;
    const QString schemaName;
    const QString tableName;

    IQueryScheduler *queryScheduler;

};

#endif // TABLEINFOVIEWERTABS_H
