#ifndef METADATALOADER_H
#define METADATALOADER_H

#include <QObject>
#include "beans/dbobjectinfo.h"
#include "connectivity/ociexception.h"
#include "interfaces/iqueryscheduler.h"

class MetadataLoader : public QObject
{
    Q_OBJECT
public:
    explicit MetadataLoader(IQueryScheduler *queryScheduler,
                            const QString &schemaName,
                            const QString &objectName,
                            QObject *parent);

    virtual ~MetadataLoader(){}

    virtual void loadObjectInfo()=0;

    QString getSchemaName() const {return this->schemaName;}
    QString getTableName() const {return this->objectName;}
    IQueryScheduler *getQueryScheduler() const {return this->queryScheduler;}
    
signals:
    void objectInfoReady(DbObjectInfo *objectInfo, MetadataLoader *loader);
    void loadError(const QString &taskName, const OciException &ex, MetadataLoader *loader);
    
protected:
    IQueryScheduler *queryScheduler;
    QString schemaName;
    QString objectName;
    
};

#endif // METADATALOADER_H
