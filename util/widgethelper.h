#ifndef WIDGETHELPER_H
#define WIDGETHELPER_H

#include <QtGui>

class Resultset;
class QListWidget;
class DataTable;

#include <QStandardItemModel>

class WidgetHelper
{
public:
    WidgetHelper();

    /*static void populateComboBox(QComboBox *comboBox,
                                 Resultset *rs,
                                 const QString &iconName,
                                 const QString &initialValue,
                                 bool prependEmptyValue=false);*/

    static void populateComboBox(QComboBox *comboBox,
                                        const QStringList itemList,
                                        const QString &iconName);

    static void setComboBoxText(QComboBox *comboBox, const QString &value);

    static void popOutTab(QTabWidget *tabWidget, int tabIndex);

    static bool moveListWidgetItems(QListWidget *from, QListWidget *to);
    static bool moveSelectedListViewItems(QListView *from, QListView *to);
    static bool moveAllListViewItems(QListView *from, QListView *to);
    static void completeLineEditText(QLineEdit *lineEdit, const QStringList &itemList);

    static void appendToPlainTextEdit(QPlainTextEdit *editor, const QString &text, bool setAllText=false, QBrush brush=QBrush());

    static void showConnectionBusyMessage();

    static QAction* addProgressBarAction(QToolBar *toolbar);

    static QLayout* layoutWidgetsInColumns(const QList< QPair<QString,QWidget*> > &widgets, int maxColumnCount);

    static void removeIncorrectRows(DataTable *table);

    static QCheckBox *createCheckBox(QLayout *layout, const QString &text, bool checked=false);
    static QCheckBox *createCheckBox(QGridLayout *layout, int row, int column, const QString &text, bool checked=false, int rowSpan=1, int colSpan=1);

    static void increaseValueAtPos(QStandardItemModel *model, int row, int column, int increaseBy);

    static void changeFontSize(QWidget *widget, int changeBy);
    static void changeFontSize(QAction *action, int changeBy);
};

#endif // WIDGETHELPER_H
