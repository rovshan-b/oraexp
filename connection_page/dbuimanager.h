#ifndef DBUIMANAGER_H
#define DBUIMANAGER_H

#include <QObject>
#include "../navtree/dbtreemodel.h"

class DbConnection;
class ConnectionPage;
class DbItemAction;

class DbUiManager : public QObject
{
    Q_OBJECT
public:
    explicit DbUiManager(DbConnection *db, QObject *parent = 0);

signals:

public slots:
    void refreshTreeNodeChildren();

    void createCreator();
    void createCreator(const QString &schemaName, const QString &objectName, const DbTreeModel::DbTreeNodeType itemType);
    void createEditor(bool editMode=true);
    void createEditor(const QString &schemaName, const QString &objectName, const DbTreeModel::DbTreeNodeType itemType, bool editMode=true);
    void createViewer();
    void createViewer(const QString &schemaName, const QString &objectName, const DbTreeModel::DbTreeNodeType itemType);

    void addWorksheet(const QString &contents="");
    void addSchemaComparer();
    void addDataComparer();

    DbItemAction *getSenderAction() const;

    void closeTab(QWidget *widget);
private:
    DbConnection *db;
    ConnectionPage *cnPage;

    QString getCorrectSchemaNameForCurrentContext();
};

#endif // DBUIMANAGER_H
