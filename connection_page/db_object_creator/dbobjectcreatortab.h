#ifndef DBOBJECTCREATORTAB_H
#define DBOBJECTCREATORTAB_H

#include <QWidget>

class IQueryScheduler;

class DbObjectCreatorTab : public QWidget
{
    Q_OBJECT
public:
    explicit DbObjectCreatorTab(bool editMode, QWidget *parent = 0);

    virtual void setQueryScheduler(IQueryScheduler *queryScheduler){this->queryScheduler=queryScheduler;}

    bool isEditMode() const {return this->editMode;}
    
protected:
    IQueryScheduler *queryScheduler;
    
private:
    bool editMode;
    
};

#endif // DBOBJECTCREATORTAB_H
