#ifndef MYTABLEWIDGETITEM_H
#define MYTABLEWIDGETITEM_H

#include <QTableWidgetItem>

class DbConnection;

class MyTableWidgetItem : public QTableWidgetItem
{
public:
    MyTableWidgetItem();
    MyTableWidgetItem(const QString &text, DbConnection *db, int type = Type);
    virtual ~MyTableWidgetItem();

    DbConnection *getDbConnection();

private:
    DbConnection *db;
};

#endif // MYTABLEWIDGETITEM_H
