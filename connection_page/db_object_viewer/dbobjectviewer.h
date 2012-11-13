#ifndef DBOBJECTVIEWER_H
#define DBOBJECTVIEWER_H

#include "../connectionpagetab.h"

class DbObjectViewerTabs;

class DbObjectViewer : public ConnectionPageTab
{
    Q_OBJECT
public:
    explicit DbObjectViewer(const QString &schemaName,
                             const QString &objectName,
                             DbUiManager *uiManager,
                             QWidget *parent = 0);

    virtual void setConnection(DbConnection *db);

    virtual void createUi();

    virtual DbObjectViewerTabs *getTabsWidget() const = 0;

protected:
    DbObjectViewerTabs *infoTabs;

    QString schemaName;
    QString objectName;

};

#endif // DBOBJECTVIEWER_H
