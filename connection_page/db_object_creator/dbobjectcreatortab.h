#ifndef DBOBJECTCREATORTAB_H
#define DBOBJECTCREATORTAB_H

#include <QWidget>

class IQueryScheduler;

template <typename ParentWidgetType>
class DbObjectCreatorTab : public QWidget
{
public:
    explicit DbObjectCreatorTab(ParentWidgetType *objectCreator, bool editMode, QWidget *parent = 0) :
        QWidget(parent),
        objectCreator(objectCreator),
        queryScheduler(0),
        editMode(editMode)
    {

    }

    virtual void setQueryScheduler(IQueryScheduler *queryScheduler){this->queryScheduler=queryScheduler;}

    bool isEditMode() const {return this->editMode;}
    
protected:
    ParentWidgetType *objectCreator;
    IQueryScheduler *queryScheduler;
    
private:
    bool editMode;
    
};

#endif // DBOBJECTCREATORTAB_H
