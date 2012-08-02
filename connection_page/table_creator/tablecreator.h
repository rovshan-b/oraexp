#ifndef TABLECREATOR_H
#define TABLECREATOR_H

#include <QWidget>
#include "../connectionpagetab.h"

class DbConnection;
class TableCreatorTabs;
class TableCreatorBottomPane;
class QSplitter;
class DbUiManager;

class TableCreator : public ConnectionPageTab
{
    Q_OBJECT
public:
    explicit TableCreator(const QString &schemaName,
                          const QString &tableName,
                          DbUiManager *uiManager,
                          QWidget *parent = 0);

    virtual void setConnection(DbConnection *db);

signals:
    void alterQuerySucceeded(const QString &taskName);

private slots:
    void updateDdlText(bool force=false);
    void ddlViewerToggled(bool visible);

    void createOrAlterTable();
    void createTable();
    void alterTable();
    void cancel();

private:
    TableCreatorTabs *tableCreatorTabs;
    TableCreatorBottomPane *bottomPane;
    QSplitter *splitter;

    QString tableName;

    bool editMode;
};

#endif // TABLECREATOR_H
