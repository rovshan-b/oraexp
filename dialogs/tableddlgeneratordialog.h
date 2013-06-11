#ifndef TABLEDDLGENERATORDIALOG_H
#define TABLEDDLGENERATORDIALOG_H

#include "codegeneratordialog.h"
#include "connectivity/queryresult.h"
#include "connectivity/fetchresult.h"

class TableDbmsDdlOptionsWidget;
class TableDdlExportOptionsWidget;
class StorageDdlExportOptionsWidget;
class QComboBox;
class DbObjectInfo;
class MetadataLoader;

class TableDdlGeneratorDialog : public CodeGeneratorDialog
{
    Q_OBJECT
public:
    explicit TableDdlGeneratorDialog(DbUiManager *uiManager,
                                     const QString &schemaName,
                                     const QString &tableName,
                                     QWidget *parent = 0);
    
    virtual void createUi();

protected:
    virtual bool isAsync() {return true;}
    virtual QString generateCode();

    virtual void setInProgress(bool inProgress);

private slots:
    void ddlQueryCompleted(const QueryResult &result);
    void ddlFetched(const FetchResult &fetchResult);
    void ddlFetchCompleted(const QString &);

    void tableInfoReady(DbObjectInfo *objectInfo, MetadataLoader *loader);
    void loadError(const QString &taskName, const OciException &ex, MetadataLoader *loader);

private:
    QString schemaName;
    QString tableName;

    QString ddl;

    QComboBox *methodCombo;
    TableDbmsDdlOptionsWidget *dbmsOptionsWidget;
    TableDdlExportOptionsWidget *appExpOptionsWidget;
    StorageDdlExportOptionsWidget *appStorageOptionsWidget;

};

#endif // TABLEDDLGENERATORDIALOG_H
