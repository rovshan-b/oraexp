#ifndef COLUMNSELECTORDIALOG_H
#define COLUMNSELECTORDIALOG_H

#include <QDialog>
#include <QStringList>

class QStandardItemModel;

namespace Ui {
    class ColumnSelectorDialog;
}

class ColumnSelectorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ColumnSelectorDialog(const QStringList &columnList,
                                  const QStringList &selColumnList,
                                  QWidget *parent,
                                  bool changeCase = true);
    ~ColumnSelectorDialog();

    QStringList getSelectedColumns() const;
    QString getSelectedColumnsCommaSeparated() const;

    void setAllowEmptySelection();

private slots:
    void on_addOneButton_clicked();
    void on_addAllButton_clicked();
    void on_removeOneButton_clicked();
    void on_removeAllButton_clicked();

private:
    Ui::ColumnSelectorDialog *ui;

    QStandardItemModel *allColumnsModel;
    QStandardItemModel *selectedColumnsModel;

    bool allowEmptySelection;

    void enableOkButton();
};

#endif // COLUMNSELECTORDIALOG_H
