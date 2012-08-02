#ifndef TABLECREATORTAB_H
#define TABLECREATORTAB_H

#include <QWidget>

class DbConnection;
class TableCreatorTabs;

class TableCreatorTab : public QWidget
{
    Q_OBJECT
public:
    explicit TableCreatorTab(TableCreatorTabs *tableCreator, bool editMode, QWidget *parent = 0);

    virtual void setConnection(DbConnection *db){this->db=db;}

    bool isEditMode() const {return this->editMode;}

    TableCreatorTabs *getTableCreator() const {return tableCreator;}

protected:
    DbConnection *db;
    TableCreatorTabs *tableCreator;

private:
    bool editMode;
};

#endif // TABLECREATORTAB_H
