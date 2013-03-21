#ifndef LINKEDOBJECTVIEWERDIALOG_H
#define LINKEDOBJECTVIEWERDIALOG_H

#include <QDialog>

class QStandardItemModel;
class DataTable;

class LinkedObjectsViewerDialog : public QDialog
{
    Q_OBJECT
public:
    explicit LinkedObjectsViewerDialog(QWidget *parent);
    
    void setObjectList(const QStringList &objectNames, const QStringList &objectTypes, const QStringList &levels, const QStringList &refTypes);

    QStandardItemModel *tableModel;
private:
    DataTable *table;
    
};

#endif // LINKEDOBJECTVIEWERDIALOG_H
