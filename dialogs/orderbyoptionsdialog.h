#ifndef ORDERBYOPTIONSDIALOG_H
#define ORDERBYOPTIONSDIALOG_H

#include <QDialog>

class QButtonGroup;

class OrderByOptionsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit OrderByOptionsDialog(QWidget *parent = 0);

public slots:
    virtual void accept ();

    QString getOrderByClause() const;

private slots:
    void removeSortChecked(bool checked);
    
private:
    enum SortDirection
    {
        Asc,
        Desc,
        Remove
    };

    enum NullsOrdering
    {
        First,
        Last,
        Default
    };

    QButtonGroup *sortDirectionGroup;
    QButtonGroup *nullsOrderingGroup;
    
};

#endif // ORDERBYOPTIONSDIALOG_H
