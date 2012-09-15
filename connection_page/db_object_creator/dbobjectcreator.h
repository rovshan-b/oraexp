#ifndef DBOBJECTCREATOR_H
#define DBOBJECTCREATOR_H

#include "../connectionpagetab.h"

class DbConnection;
class DbUiManager;
class DbObjectCreatorPane;
class DbObjectCreatorBottomPane;
class QSplitter;

class DbObjectCreator : public ConnectionPageTab
{
    Q_OBJECT
public:
    explicit DbObjectCreator(const QString &schemaName,
                             const QString &objectName,
                             DbUiManager *uiManager,
                             QWidget *parent = 0);
    
    virtual void createUi();

    virtual void setConnection(DbConnection *db);

protected:
    virtual DbObjectCreatorPane *createCreatorPane(const QString &schemaName, const QString &objectName)=0;

private slots:
    void updateDdlText(bool force=false);
    void ddlViewerToggled(bool visible);

    void createOrAlterObject();
    void createObject();
    void alterObject();
    void cancel();

private:
    QString schemaName;
    QString objectName;

    DbObjectCreatorPane *creatorPane;
    DbObjectCreatorBottomPane *bottomPane;
    QSplitter *splitter;

    bool editMode;
    
};

#endif // DBOBJECTCREATOR_H
