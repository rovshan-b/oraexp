#ifndef DBOBJECTCREATOR_H
#define DBOBJECTCREATOR_H

#include <QQueue>

#include "../connectionpagetab.h"
#include "util/triple.h"
#include "connectivity/queryresult.h"
#include "navtree/dbtreemodel.h"

class DbConnection;
class DbUiManager;
class DbObjectCreatorPane;
class DbObjectCreatorBottomPane;
class QSplitter;

class DbObjectCreator : public ConnectionPageTab
{
    Q_OBJECT
public:
    enum CreatorMode
    {
        CreateNew,
        EditExisting,
        CreateLike
    };

    explicit DbObjectCreator(const QString &schemaName,
                             const QString &objectName,
                             DbUiManager *uiManager,
                             DbTreeModel::DbTreeNodeType objectType,
                             CreatorMode creatorMode,
                             QWidget *parent);
    
    virtual void createUi();

    virtual void setConnection(DbConnection *db);

    QString getOriginalSchemaName() const {return this->schemaName;}
    QString getOriginalObjectName() const {return this->objectName;}
    DbTreeModel::DbTreeNodeType getObjectType() const {return this->objectType;}

    void setOriginalSchemaName(const QString &schemaName) {this->schemaName = schemaName;}
    void setOriginalObjectName(const QString &objectName) {this->objectName = objectName;}

    CreatorMode getCreatorMode() const {return this->creatorMode;}

protected:
    virtual DbObjectCreatorPane *createCreatorPane(DbObjectCreator *objectCreator)=0;

public slots:
    void createQueryCompleted(const QueryResult &result);
    void alterQueryCompleted(const QueryResult &result);

private slots:
    void updateDdlText(bool force=false);
    void ddlViewerToggled(bool visible);

    void createOrAlterObject();
    void createObject();
    void alterObject();
    void cancel();

protected:
    DbObjectCreatorPane *creatorPane;

private:
    QString schemaName;
    QString objectName;
    DbTreeModel::DbTreeNodeType objectType;

    DbObjectCreatorBottomPane *bottomPane;
    QSplitter *splitter;

    CreatorMode creatorMode;

    void startProgress();
    void stopProgress();

    void executeNextCreateQuery();
    void processCreateError(const OciException &ex);
    QStringList createDdlList;

    void executeNextAlterQuery();
    void processAlterSuccess();
    void processAlterError(const OciException &ex);

    QQueue < Triple<QPointer<QObject>, QString, QString> > alterDdlQueue;
    Triple<QPointer<QObject>, QString, QString> currentAlterItem;
    int alterQueryIx;
    
};

#endif // DBOBJECTCREATOR_H
