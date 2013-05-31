#ifndef DBOBJECTCREATORSIMPLEPANE_H
#define DBOBJECTCREATORSIMPLEPANE_H

#include "dbobjectcreatorpane.h"
#include "beans/dbobjectinfo.h"
#include "connectivity/ociexception.h"
#include "connection_page/db_object_creator/dbobjectcreator.h"

class DbItemListComboBox;
class MetadataLoader;

class DbObjectCreatorSimplePane : public DbObjectCreatorPane
{
    Q_OBJECT
public:
    explicit DbObjectCreatorSimplePane(DbObjectCreator *objectCreator, QWidget *parent = 0);
    ~DbObjectCreatorSimplePane();
    
    virtual void createUi();
    virtual QLayout *createForm()=0;

    virtual void setQueryScheduler(IQueryScheduler *queryScheduler);

    DbObjectCreator::CreatorMode getCreatorMode() const;

private slots:
    void objectInfoReady(DbObjectInfo *objectInfo, MetadataLoader *loader);
    void loadError(const QString &taskName, const OciException &ex, MetadataLoader *loader);

protected:
    virtual DbItemListComboBox *schemaListCombo() const=0;

    virtual void disableControlsForEditMode(){}

    DbObjectInfo *originalObjectInfo;

    template<class T>  T *getOriginalObjectInfo() const
    {
        Q_ASSERT(originalObjectInfo);
        T *info = static_cast<T*>(originalObjectInfo);

        return info;
    }

    virtual void setObjectInfo(DbObjectInfo *objectInfo)=0;
    
};

#endif // DBOBJECTCREATORSIMPLEPANE_H
