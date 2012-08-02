#ifndef TABLEINFOVIEWER_H
#define TABLEINFOVIEWER_H

#include "../connectionpagetab.h"

class TableInfoViewerTabs;

class TableInfoViewer : public ConnectionPageTab
{
    Q_OBJECT
public:
    explicit TableInfoViewer(const QString &schemaName,
                             const QString &tableName,
                             DbUiManager *uiManager,
                             QWidget *parent = 0);

    virtual void setConnection(DbConnection *db);

private:
    TableInfoViewerTabs *infoTabs;

    QString schemaName;
    QString tableName;

};

#endif // TABLEINFOVIEWER_H
