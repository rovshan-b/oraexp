#ifndef DBOBJECTCREATORTAB_H
#define DBOBJECTCREATORTAB_H

#include <QWidget>
#include "connection_page/db_object_creator/dbobjectcreator.h"

class IQueryScheduler;

template <typename ParentWidgetType>
class DbObjectCreatorTab : public QWidget
{
public:
    explicit DbObjectCreatorTab(ParentWidgetType *objectCreator, DbObjectCreator::CreatorMode creatorMode, QWidget *parent = 0) :
        QWidget(parent),
        objectCreator(objectCreator),
        queryScheduler(0),
        creatorMode(creatorMode)
    {

    }

    virtual void setQueryScheduler(IQueryScheduler *queryScheduler){this->queryScheduler=queryScheduler;}

    DbObjectCreator::CreatorMode getCreatorMode() const {return this->creatorMode;}
    
protected:
    ParentWidgetType *objectCreator;
    IQueryScheduler *queryScheduler;
    
private:
    DbObjectCreator::CreatorMode creatorMode;
    
};

#endif // DBOBJECTCREATORTAB_H
