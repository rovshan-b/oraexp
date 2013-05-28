#ifndef CHECKEDLISTBOXWIDGET_H
#define CHECKEDLISTBOXWIDGET_H

#include <QWidget>

class QListView;
class QPushButton;
class QStandardItemModel;

class CheckedListBoxWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CheckedListBoxWidget(QWidget *parent = 0);
    
    QStandardItemModel *itemsModel() const {return this->model;}

    void addItem(const QPixmap &pixmap, const QString &text, bool checked);

    QStringList getCheckedItemNames() const;

private slots:
    void selectAll();
    void selectNone();

private:
    QListView *listView;
    QPushButton *selectAllButton;
    QPushButton *selectNoneButton;

    QStandardItemModel *model;
    
};

#endif // CHECKEDLISTBOXWIDGET_H
