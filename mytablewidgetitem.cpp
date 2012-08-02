#include "mytablewidgetitem.h"
#include "connectivity/dbconnection.h"

MyTableWidgetItem::MyTableWidgetItem()
{
}

MyTableWidgetItem::MyTableWidgetItem(const QString &text, DbConnection *db, int type) : QTableWidgetItem(text, type)
{
    this->db=db;
}

MyTableWidgetItem::~MyTableWidgetItem()
{
    delete db;
}

DbConnection *MyTableWidgetItem::getDbConnection()
{
    return db;
}
