#ifndef WIDGETHELPER_H
#define WIDGETHELPER_H

#include "navtree/dbtreemodel.h"
#include <QtGui>

class Resultset;
class QListWidget;
class DataTable;
class GenericEditableTableModel;
class CodeEditor;

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
    static QString getComboBoxUserDataOrText(QComboBox *comboBox);

    static void popOutTab(QTabWidget *tabWidget, int tabIndex);

    static bool moveListWidgetItems(QListWidget *from, QListWidget *to);
    static bool moveSelectedListViewItems(QListView *from, QListView *to);
    static bool moveAllListViewItems(QListView *from, QListView *to);
    static void completeLineEditText(QLineEdit *lineEdit, const QStringList &itemList);

    static void appendToPlainTextEdit(QPlainTextEdit *editor, const QString &text, bool setAllText=false, QBrush brush=QBrush());

    static void showConnectionBusyMessage();

    static QAction* addProgressBarAction(QToolBar *toolbar, bool visible=false);

    static QLayout* layoutWidgetsInColumns(const QList< QPair<QString,QWidget*> > &widgets, int maxColumnCount);

    static void removeIncorrectRows(DataTable *table);

    static QGridLayout *createCheckBoxes(const QStringList &texts, int columnCount, const QStringList &checked=QStringList());
    static QCheckBox *createCheckBox(QLayout *layout, const QString &text, bool checked=false);
    static QCheckBox *createCheckBox(QGridLayout *layout, int row, int column, const QString &text, bool checked=false, int rowSpan=1, int colSpan=1);
    static QStringList getCheckedBoxes(QWidget *parent);
    static void setCheckedBoxes(QWidget *parent, const QStringList &checkedList, bool check=true, bool all=false);

    static void increaseValueAtPos(QStandardItemModel *model, int row, int column, int increaseBy);

    static void changeFontSize(QWidget *widget, qreal changeBy);
    static void changeFontSize(QAction *action, qreal changeBy);

    static QAction *addStretchToToolbar(QToolBar *toolbar);

    static void updateActionTooltips(QWidget *widget);

    static void invokeSlot(QWidget *w, const char *slotName);

    static void addDbItemAction(QMenu *menu, const QIcon &icon, const QString &text,
                                const QString &schemaName,
                                const QString &objectName,
                                const DbTreeModel::DbTreeNodeType itemType,
                                const QObject *receiver=0,
                                const char *slotName=0,
                                QKeySequence shortcut = QKeySequence());

    template <class T> static bool validate(T *obj, bool editMode, QWidget *window)
    {
        QStringList msgs=obj->validate(editMode);
        if(msgs.size()>0){
            QString msg=QObject::tr("Following validation error(s) occured:\n");
            foreach(const QString &m, msgs){
                msg.append("  - ").append(m).append("\n");
            }
            msg.chop(1);

            QMessageBox::critical(window, QObject::tr("Validation error(s) occured"), msg);
        }
        return msgs.size()==0;
    }

    static void createCheckUncheckButtons(QGridLayout *layout,
                                       const QObject *receiver,
                                       const char *checkSlotName,
                                       const char *uncheckSlotName);

    static void deleteTableRow(DataTable *table, int rowIx=-1, bool selection=true, bool prompt=true);
    static void selectTableRowAfterDeletion(DataTable *table, GenericEditableTableModel *model, int prevRowIndex, int colIndex);
    static QRegExp createDateTimeRegExp();

    static QFont getMonospaceFont(int pointSize=-1);

    static void fillAvailableTextCodecNames(QComboBox *comboBox, const QString &preselect = "UTF-8");

    static void checkModelItems(QStandardItemModel *model, bool check);
};

#endif // WIDGETHELPER_H
