#ifndef TABLECREATORTAB_H
#define TABLECREATORTAB_H

#include <QWidget>

class IQueryScheduler;
class TableCreatorTabs;

class TableCreatorTab : public QWidget
{
    Q_OBJECT
public:
    explicit TableCreatorTab(TableCreatorTabs *tableCreator, bool editMode, QWidget *parent = 0);

    virtual void setQueryScheduler(IQueryScheduler *queryScheduler){this->queryScheduler=queryScheduler;}

    bool isEditMode() const {return this->editMode;}

    TableCreatorTabs *getTableCreator() const {return tableCreator;}

protected:
    IQueryScheduler *queryScheduler;
    TableCreatorTabs *tableCreator;

private:
    bool editMode;
};

#endif // TABLECREATORTAB_H
