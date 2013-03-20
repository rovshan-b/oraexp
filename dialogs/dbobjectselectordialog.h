#ifndef DBOBJECTSELECTORDIALOG_H
#define DBOBJECTSELECTORDIALOG_H

#include <QDialog>

class QStandardItemModel;
class DataTable;

class DbObjectSelectorDialog : public QDialog
{
    Q_OBJECT
public:
    explicit DbObjectSelectorDialog(QWidget *parent);
    
    void setObjectList(const QStringList &objectNames, const QStringList &objectTypes, const QStringList &levels);

private:
    QStandardItemModel *tableModel;
    DataTable *table;
    
};

#endif // DBOBJECTSELECTORDIALOG_H
