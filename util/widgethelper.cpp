#include "widgethelper.h"
#include "connectivity/dbconnection.h"
#include "connectivity/resultset.h"
#include "connectivity/ociexception.h"
#include "util/iconutil.h"
#include "models/genericeditabletablemodel.h"
#include "widgets/datatable.h"
#include <QLineEdit>
#include <QListWidget>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QMessageBox>
#include <memory>

using namespace std;

WidgetHelper::WidgetHelper()
{
}
/*
void WidgetHelper::populateComboBox(QComboBox *comboBox,
                                    Resultset *rs,
                                    const QString &iconName,
                                    const QString &initialValue,
                                    bool prependEmptyValue)
{
    comboBox->setEditable(true);
    QString initValue=initialValue.isEmpty() ? comboBox->lineEdit()->text() : initialValue;

    bool hasSelection=comboBox->lineEdit()->hasSelectedText();

    comboBox->clear();

    try{
        auto_ptr<Resultset> autoRs(rs);

        QIcon icon=IconUtil::getIcon(iconName);

        if(prependEmptyValue){
            comboBox->addItem(icon, "");
        }

        rs->beginFetchRows();
        while(rs->moveNext()){
            comboBox->addItem(icon, rs->getString(1));
        }
        rs->endFetchRows();
    }
    catch(OciException &ex){
        QMessageBox::critical(comboBox->window(), QObject::tr("Error while populating combo box"),
                              ex.getErrorMessage());
    }

    if(!initValue.isEmpty()){
        setComboBoxText(comboBox, initValue);

        if(hasSelection){
            comboBox->lineEdit()->selectAll();
        }
    }
}*/

void WidgetHelper::populateComboBox(QComboBox *comboBox,
                                    const QStringList itemList,
                                    const QString &iconName)
{
    QIcon icon=IconUtil::getIcon(iconName);
    Q_ASSERT(comboBox->isEditable());
    QString currentText=comboBox->lineEdit()->text();

    int itemCount=itemList.size();
    for(int i=0; i<itemCount; ++i){
        comboBox->addItem(icon, itemList.at(i));
    }

    WidgetHelper::setComboBoxText(comboBox, currentText);
}

void WidgetHelper::setComboBoxText(QComboBox *comboBox, const QString &value){
    int editorItemIndex=comboBox->findText(value, Qt::MatchFixedString);
    if(editorItemIndex!=-1){
        comboBox->setCurrentIndex(editorItemIndex);
    }else if(comboBox->isEditable()){
        comboBox->lineEdit()->setText(value);
    }else{
        comboBox->addItem(value);
        comboBox->setCurrentIndex(comboBox->count()-1);
    }
}

void WidgetHelper::popOutTab(QTabWidget *tabWidget, int tabIndex)
{
    QWidget *currentTabWidget=tabWidget->currentWidget();

    QString title=tabWidget->tabText(tabIndex);
    QIcon icon=tabWidget->tabIcon(tabIndex);
    tabWidget->removeTab(tabIndex);

    QMainWindow *popOutWindow=new QMainWindow(tabWidget);
    popOutWindow->setAttribute(Qt::WA_DeleteOnClose);
    popOutWindow->setWindowTitle(title);
    popOutWindow->setWindowIcon(icon);
    popOutWindow->resize(700, 500);

    currentTabWidget->setParent(popOutWindow);
    popOutWindow->setCentralWidget(currentTabWidget);
    currentTabWidget->show();

    popOutWindow->show();
    popOutWindow->activateWindow();
    popOutWindow->raise();
}

bool sortModelIndexesByRow(const QModelIndex &ix1, const QModelIndex &ix2)
{
     return ix1.row() < ix2.row();
}

bool WidgetHelper::moveListWidgetItems(QListWidget *from, QListWidget *to)
{
    QItemSelectionModel *selModel=from->selectionModel();
    QModelIndexList selIndexes = selModel->selectedIndexes();

    if(selIndexes.count()<1){
        return false;
    }

    qSort(selIndexes.begin(), selIndexes.end(), sortModelIndexesByRow);
    int topSelectedIx=0;
    if(selIndexes.size()>0){
        topSelectedIx=selIndexes.at(0).row();
    }
    int selectedCount = selIndexes.size();
    int lastItemInSelectedColumns = to->count();
    for(int i=selectedCount-1; i>=0; --i){
        to->insertItem(lastItemInSelectedColumns,
                        from->takeItem(selIndexes.at(i).row())
                    );
    }

    if(!selModel->hasSelection()){
        int itemCount = from->count();
        if(itemCount>0){
            int indexToSelect=topSelectedIx<itemCount ? topSelectedIx : itemCount-1;

            selModel->select(from->model()->index(indexToSelect, 0), QItemSelectionModel::Select);
        }
    }

    return true;
}


bool WidgetHelper::moveSelectedListViewItems(QListView *from, QListView *to)
{
    QItemSelectionModel *selModel=from->selectionModel();
    QModelIndexList selIndexes = selModel->selectedIndexes();

    if(selIndexes.count()<1){
        return false;
    }

    QStandardItemModel *fromModel=static_cast<QStandardItemModel *>(from->model());
    QStandardItemModel *toModel=static_cast<QStandardItemModel *>(to->model());

    qSort(selIndexes.begin(), selIndexes.end(), sortModelIndexesByRow);
    int topSelectedIx=0;
    if(selIndexes.size()>0){
        topSelectedIx=selIndexes.at(0).row();
    }
    int selectedCount = selIndexes.size();
    int lastItemInTargetList = toModel->rowCount();

    for(int i=selectedCount-1; i>=0; --i){
        toModel->insertRow(lastItemInTargetList,
                        fromModel->takeRow(selIndexes.at(i).row())
                    );
    }

    if(!selModel->hasSelection()){
        int itemCount = fromModel->rowCount();
        if(itemCount>0){
            int indexToSelect=topSelectedIx<itemCount ? topSelectedIx : itemCount-1;

            selModel->select(from->model()->index(indexToSelect, 0), QItemSelectionModel::Select);
        }
    }

    return true;
}

bool WidgetHelper::moveAllListViewItems(QListView *from, QListView *to)
{
    QStandardItemModel *fromModel=static_cast<QStandardItemModel *>(from->model());
    QStandardItemModel *toModel=static_cast<QStandardItemModel *>(to->model());

    int itemCountInSourceList = fromModel->rowCount();
    if(itemCountInSourceList<1){
        return false;
    }

    int lastItemInTargetList = toModel->rowCount();

    for(int i=itemCountInSourceList-1; i>=0; --i){
        toModel->insertRow(lastItemInTargetList,
                        fromModel->takeRow(i)
                    );
    }

    return true;
}

void WidgetHelper::completeLineEditText(QLineEdit *lineEdit, const QStringList &itemList)
{
    QString currentText=lineEdit->text().trimmed();
    if(!currentText.isEmpty() && !itemList.empty()){

        int cursorPos=lineEdit->cursorPosition();
        int prevCommaIndex=currentText.mid(0, cursorPos).lastIndexOf(',');
        int nextCommaIndex=currentText.indexOf(',', prevCommaIndex==-1 ? 0 : prevCommaIndex+1);

        if(prevCommaIndex!=-1 && nextCommaIndex!=-1){
            currentText=currentText.mid(prevCommaIndex+1, nextCommaIndex-prevCommaIndex-1).trimmed();
        }else if(prevCommaIndex!=-1){
            currentText=currentText.mid(prevCommaIndex+1).trimmed();
        }else if(nextCommaIndex!=-1){
            currentText=currentText.mid(0, nextCommaIndex).trimmed();
        }

        if(currentText.isEmpty()){
            return;
        }

        int colCount=itemList.count();
        QString currentColName;
        for(int i=0; i<colCount; ++i){
            currentColName=itemList.at(i);
            if(currentColName.startsWith(currentText, Qt::CaseInsensitive)){

                if(currentColName.length()==currentText.length()){
                    break;
                }

                QString textToAppend=currentColName.mid(currentText.length());

                int cursorPosition=lineEdit->cursorPosition();
                lineEdit->insert(textToAppend);
                lineEdit->setSelection(cursorPosition, textToAppend.length());

                break;
            }
        }
    }
}

void WidgetHelper::appendToPlainTextEdit(QPlainTextEdit *editor, const QString &text, bool setAllText, QBrush brush)
{
    editor->setUpdatesEnabled(false);

    int currScrollPos=editor->verticalScrollBar()->value();
    int maxValue=editor->verticalScrollBar()->maximum();

    QTextCursor cursor=editor->textCursor();
    cursor.clearSelection();
    QTextCharFormat format;
    format.setForeground(brush);
    //if(brush.style()!=Qt::NoBrush){
    //    format.setFontWeight(500);
    //}
    cursor.setCharFormat(format);
    editor->setTextCursor(cursor);

    if(setAllText){
        editor->setPlainText(text);
    }else{
        editor->appendPlainText(text);
    }

    if(currScrollPos==maxValue){
        currScrollPos=editor->verticalScrollBar()->maximum();
    }
    editor->verticalScrollBar()->setValue(currScrollPos);

    editor->setUpdatesEnabled(true);
}

void WidgetHelper::showConnectionBusyMessage()
{
    QMessageBox::critical(NULL, QObject::tr("Connection busy"), QObject::tr("There is already an ongoing operation on this connection. "
                                                          "Please, wait for it to complete, and try again."));
}

QAction* WidgetHelper::addProgressBarAction(QToolBar *toolbar, bool visible)
{
    QProgressBar *progressBar=new QProgressBar();
    progressBar->setRange(0,0);
    progressBar->setMaximumWidth(100);
    progressBar->setMaximumHeight(16);
    progressBar->setVisible(visible);
    return toolbar->addWidget(progressBar);
}

QLayout* WidgetHelper::layoutWidgetsInColumns(const QList< QPair<QString,QWidget*> > &widgets, int maxColumnCount)
{
    int colCount=qMin(widgets.size(), maxColumnCount);
    int widgetsPerCol=qCeil(((qreal)widgets.size())/colCount);

    QHBoxLayout *layout=new QHBoxLayout();

    QFormLayout *form;
    for(int i=0; i<widgets.size(); ++i){
        if(i%widgetsPerCol==0){
            form=new QFormLayout();
            layout->addLayout(form);
        }

        form->addRow(widgets.at(i).first, widgets.at(i).second);
    }

    return layout;
}

void WidgetHelper::removeIncorrectRows(DataTable *table)
{
    static_cast<GenericEditableTableModel*>(table->model())->removeIncorrectRows();
    table->closePersistentEditor(table->currentIndex());
}

QCheckBox *WidgetHelper::createCheckBox(QLayout *layout, const QString &text, bool checked)
{
    QCheckBox *chk=new QCheckBox(text);
    chk->setChecked(checked);
    layout->addWidget(chk);

    return chk;
}

QCheckBox *WidgetHelper::createCheckBox(QGridLayout *layout, int row, int column, const QString &text, bool checked, int rowSpan, int colSpan)
{
    QCheckBox *chk=new QCheckBox(text);
    chk->setChecked(checked);
    layout->addWidget(chk, row, column, rowSpan, colSpan);

    return chk;
}

void WidgetHelper::increaseValueAtPos(QStandardItemModel *model, int row, int column, int increaseBy)
{
    QStandardItem *item = model->item(row, column);
    item->setText(QString::number(item->text().toInt()+increaseBy));
}

void WidgetHelper::changeFontSize(QWidget *widget, int changeBy)
{
    QFont font=widget->font();
    font.setPointSize(font.pointSize()+changeBy);
    widget->setFont(font);
}

void WidgetHelper::changeFontSize(QAction *action, int changeBy)
{
    QFont font=action->font();
    font.setPointSize(font.pointSize()+changeBy);
    action->setFont(font);
}

void WidgetHelper::addStretchToToolbar(QToolBar *toolbar)
{
    QWidget *widget=new QWidget();
    widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    toolbar->addWidget(widget);
}

void WidgetHelper::updateActionTooltips(QWidget *widget)
{
    QKeySequence shortcut;
    foreach(QAction *action, widget->actions()){
        shortcut = action->shortcut();
        if(!shortcut.isEmpty()){
            action->setToolTip(QString("%1 (%2)").arg(action->toolTip(), shortcut.toString()));
        }
    }
}

