#include "widgethelper.h"
#include "connectivity/dbconnection.h"
#include "connectivity/resultset.h"
#include "connectivity/ociexception.h"
#include "util/iconutil.h"
#include "models/genericeditabletablemodel.h"
#include "widgets/datatable.h"
#include "widgets/autorefreshcombobox.h"
#include "codeeditor/codeeditor.h"
#include <QLineEdit>
#include <QListWidget>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QMessageBox>
#include <QMenu>
#include "beans/dbitemaction.h"
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

QString WidgetHelper::getComboBoxUserDataOrText(QComboBox *comboBox)
{
    QString result;
    int currentIx = comboBox->currentIndex();
    if(currentIx!=-1){
        if(comboBox->isEditable()){
            result = comboBox->lineEdit()->text();
            int foundIx = comboBox->findText(result);
            if(foundIx!=-1){
                result = comboBox->itemData(foundIx).toString();
            }
        }else{
            result = comboBox->itemData(currentIx).toString();
        }

    }else{
        result = comboBox->currentText();
    }

    return result;
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

void WidgetHelper::enableToolBarActions(QToolBar *toolbar, bool enable)
{
    foreach(QAction *action, toolbar->actions()){
        action->setEnabled(enable);
    }
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

QGridLayout *WidgetHelper::createCheckBoxes(const QStringList &texts, int columnCount, const QStringList &checked)
{
    QGridLayout *layout=new QGridLayout();

    int rowCount = qCeil(((qreal)texts.size())/columnCount);
    int currentRow=0;
    int currentColumn=0;
    for(int i=0; i<texts.size(); ++i){
        if(i>0 && i%rowCount==0){
            currentRow=0;
            ++currentColumn;
        }
        WidgetHelper::createCheckBox(layout, currentRow++, currentColumn, texts.at(i), checked.contains(texts.at(i)));
    }

    return layout;
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

QCheckBox *WidgetHelper::createCheckBox(QFormLayout *layout, const QString &text, bool checked)
{
    QCheckBox *chk=new QCheckBox(text);
    chk->setChecked(checked);
    layout->addRow(chk);

    return chk;
}

QStringList WidgetHelper::getCheckedBoxes(QWidget *parent)
{
    QStringList results;

    QList<QCheckBox*> children=parent->findChildren<QCheckBox*>();
    foreach(const QCheckBox *chk, children){
        if(chk->isChecked()){
            results.append(chk->text());
        }
    }

    return results;
}

void WidgetHelper::setCheckedBoxes(QWidget *parent, const QStringList &checkedList, bool check, bool all)
{
    QList<QCheckBox*> children=parent->findChildren<QCheckBox*>();
    foreach(QCheckBox *chk, children){
        if(chk->isChecked()!=check && (all || checkedList.contains(chk->text()))){
            chk->setChecked(check);
        }
    }
}

void WidgetHelper::increaseValueAtPos(QStandardItemModel *model, int row, int column, int increaseBy)
{
    QStandardItem *item = model->item(row, column);
    item->setText(QString::number(item->text().toInt()+increaseBy));
}

void WidgetHelper::changeFontSize(QWidget *widget, qreal changeBy)
{
    QFont font=widget->font();

    qreal newFontSize=font.pointSizeF()+changeBy;
    if(newFontSize<1 || newFontSize>70){
        return;
    }
    font.setPointSizeF(newFontSize);
    widget->setFont(font);
}

void WidgetHelper::changeFontSize(QAction *action, qreal changeBy)
{
    QFont font=action->font();
    font.setPointSizeF(font.pointSizeF()+changeBy);
    action->setFont(font);
}

QAction* WidgetHelper::addStretchToToolbar(QToolBar *toolbar)
{
    QWidget *widget=new QWidget();
    widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    return toolbar->addWidget(widget);
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

void WidgetHelper::invokeSlot(QWidget *w, const char *slotName)
{
    if(!w){
        return;
    }

    w->metaObject()->invokeMethod(w, slotName, Qt::DirectConnection);
}

void WidgetHelper::addDbItemAction(QMenu *menu, const QIcon &icon, const QString &text, const QString &schemaName, const QString &objectName, const DbTreeModel::DbTreeNodeType itemType, const QObject *receiver, const char *slotName, QKeySequence shortcut)
{
    DbItemAction *action = new DbItemAction(icon, text, schemaName, objectName, itemType, receiver, slotName, menu);
    if(!shortcut.isEmpty()){
        action->setShortcut(shortcut);
    }
    menu->addAction(action);
}

void WidgetHelper::createCheckUncheckButtons(QGridLayout *layout,
                                          const QObject *receiver,
                                          const char *checkSlotName,
                                          const char *uncheckSlotName)
{
    QHBoxLayout *buttonsLayout = WidgetHelper::addCheckUncheckButtons(receiver, checkSlotName, uncheckSlotName);

    int rowCount=layout->rowCount();
    int columnCount=layout->columnCount();

    layout->addLayout(buttonsLayout, rowCount, 0, 1, columnCount, Qt::AlignLeft);
}

QHBoxLayout *WidgetHelper::addCheckUncheckButtons(const QObject *receiver,
                                             const char *checkSlotName,
                                             const char *uncheckSlotName)
{
    QPushButton *checkButton = new QPushButton(QObject::tr("Select all"));
    QPushButton *uncheckButton = new QPushButton(QObject::tr("Select none"));

    QObject::connect(checkButton, SIGNAL(clicked()), receiver, checkSlotName);
    QObject::connect(uncheckButton, SIGNAL(clicked()), receiver, uncheckSlotName);

    QHBoxLayout *buttonsLayout=new QHBoxLayout();
    buttonsLayout->addWidget(checkButton);
    buttonsLayout->addWidget(uncheckButton);
    buttonsLayout->addStretch();

    return buttonsLayout;
}

void WidgetHelper::deleteTableRow(DataTable *table, int rowIx, bool selection, bool prompt)
{
    QModelIndex currentIndex=selection ? table->currentIndex() : table->model()->index(rowIx,0);
    if(currentIndex.isValid()){

        int row=currentIndex.row();
        int prevRowIndex=row>0 ? row-1 : 0;
        int colIndex=currentIndex.column();

        QItemSelectionModel *selModel=table->selectionModel();
        Q_ASSERT(selModel);

        GenericEditableTableModel *model=qobject_cast<GenericEditableTableModel*>(table->model());
        Q_ASSERT(model);

        bool prompted=prompt ? false : true; //if prompting is not required set prompted = true, as if we already prompted user

        int rowCount=model->rowCount();
        for(int i=rowCount-1; i>=0; --i){
            if((selection && selModel->rowIntersectsSelection(i, QModelIndex())) || i==row){

                if(!prompted && QMessageBox::question(table->window(),
                                                      QObject::tr("Confirm deletion"),
                                                      QObject::tr("Delete selected rows?"),
                                                      QMessageBox::Ok,
                                                      QMessageBox::Cancel)!=QMessageBox::Ok){
                    return;
                }else{
                    prompted=true;
                }

                if(model->isRowFrozen(i)){
                    if(!model->isRowDeleted(i)){
                        model->markRowAsDeleted(i);
                    }
                }else{
                    model->removeRows(i, 1);
                }
            }
        }

        WidgetHelper::selectTableRowAfterDeletion(table, model, prevRowIndex, colIndex);
    }
}

void WidgetHelper::selectTableRowAfterDeletion(DataTable *table, GenericEditableTableModel *model, int prevRowIndex, int colIndex)
{
    if(model->rowCount()<1){
        return;
    }

    bool selectionMade=false;
    for(int i=prevRowIndex; i>=0; --i){
        if(!model->isRowDeleted(i)){
            table->selectionModel()->setCurrentIndex(model->index(i, colIndex), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Current);
            selectionMade=true;
            break;
        }
    }

    if(!selectionMade){
        for(int i=prevRowIndex; i<model->rowCount(); ++i){
            if(!model->isRowDeleted(i)){
                table->selectionModel()->setCurrentIndex(model->index(i, colIndex), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Current);
                break;
            }
        }
    }
}

QRegExp WidgetHelper::createDateTimeRegExp()
{
    return QRegExp("[0-9]{4}-(((0[13578]|(10|12))-(0[1-9]|[1-2][0-9]|3[0-1]))|(02-(0[1-9]|[1-2][0-9]))|((0[469]|11)-(0[1-9]|[1-2][0-9]|30)))( ([01]\\d|2[0123]):([0-5]\\d):([0-5]\\d))?",
                   Qt::CaseSensitive, QRegExp::RegExp2);
}

QFont WidgetHelper::getMonospaceFont(int pointSize)
{
    QFont f("Monospace", pointSize);
    f.setStyleHint(QFont::TypeWriter);
    return f;
}

void WidgetHelper::fillAvailableTextCodecNames(QComboBox *comboBox, const QString &preselect)
{
    QList<QByteArray> codecs = QTextCodec::availableCodecs();
    foreach(QByteArray codec, codecs){
        comboBox->addItem(codec);
    }

    QStandardItemModel *model=qobject_cast<QStandardItemModel*>(comboBox->model());
    if(model){
        model->sort(0);
    }
    comboBox->setCurrentIndex(comboBox->findText(preselect));
}

void WidgetHelper::checkModelItems(QStandardItemModel *model, bool check)
{
    for(int i=0; i<model->rowCount(); ++i){
        model->item(i, 0)->setCheckState(check ? Qt::Checked : Qt::Unchecked);
    }
}

QGroupBox *WidgetHelper::createGroupBox(QLayout *layout, const QString &title)
{
    QGroupBox *groupBox=new QGroupBox(title);
    groupBox->setLayout(layout);

    return groupBox;
}

QGroupBox *WidgetHelper::createGroupBox(QWidget *widget, const QString &title)
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(widget);

    return WidgetHelper::createGroupBox(layout, title);
}

QScrollArea *WidgetHelper::createScrollArea(QLayout *layout)
{
    QWidget *scrollWidget=new QWidget();
    scrollWidget->setLayout(layout);

    return WidgetHelper::createScrollArea(scrollWidget);
}

QScrollArea *WidgetHelper::createScrollArea(QWidget *widget)
{
    QScrollArea *scrollArea=new QScrollArea();
    widget->setBackgroundRole(QPalette::Base);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widget);

    return scrollArea;
}

QWidget *WidgetHelper::findParentWidget(QWidget *w, const QString &parentWidgetClassName, int maxLevelsUp)
{
    QWidget *tmp = w;

    for(int i=0; i<maxLevelsUp; ++i){
        tmp = tmp->parentWidget();
        if(tmp==0){
            return 0;
        }

        if(tmp->metaObject()->className()==parentWidgetClassName){
            return tmp;
        }
    }

    return 0;
}

void WidgetHelper::fillFieldDelimiters(QComboBox *comboBox)
{
    comboBox->setEditable(true);
    comboBox->addItem(QObject::tr(","), ",");
    comboBox->addItem(QObject::tr(":"), ":");
    comboBox->addItem(QObject::tr(";"), ";");
    comboBox->addItem(QObject::tr("|"), "|");
    comboBox->addItem(QObject::tr("Tab"), "\t");
    comboBox->addItem(QObject::tr("Space"), " ");
}

void WidgetHelper::deleteViewModel(QAbstractItemView *view)
{
    QAbstractItemModel *currentModel=view->model();
    QItemSelectionModel *currentSelectionModel=view->selectionModel();

    if(currentModel!=0){
        delete currentModel;
    }

    if(currentSelectionModel!=0){
        delete currentSelectionModel;
    }
}

QAction *WidgetHelper::createSeparatorAction(QObject *parent)
{
    QAction *action = new QAction(parent);
    action->setSeparator(true);
    return action;
}

void WidgetHelper::deleteMenu(QMenu *menu)
{
    QList<QAction *> actions = menu->actions();
    WidgetHelper::deleteActions(actions);

    delete menu;
}

void WidgetHelper::deleteActions(QList<QAction *> &actions)
{
    for(int i=actions.size()-1; i>=0; --i){
        QAction *action = actions.at(i);
        if(action->menu()){
            WidgetHelper::deleteMenu(action->menu());
        }
        delete action;
    }
}

QWidget *WidgetHelper::compactWidget(QWidget *widget)
{
    return WidgetHelper::compactWidgets(QList<QWidget *>() << widget);
}

QWidget *WidgetHelper::compactWidgets(QList<QWidget *> widgets, Qt::Orientation orientation)
{
    QWidget *widget = new QWidget();
    QBoxLayout *layout;
    if(orientation==Qt::Horizontal){
        layout = new QHBoxLayout();
    }else{
        layout = new QVBoxLayout();
    }

    foreach(QWidget *w, widgets){
        layout->addWidget(w, 0, Qt::AlignLeft | Qt::AlignTop);
    }

    layout->setContentsMargins(0,0,0,0);
    widget->setLayout(layout);

    return widget;
}

QWidget *WidgetHelper::createMessageWidget(const QString &message)
{
    QWidget *widget = new QWidget();

    QHBoxLayout *layout = new QHBoxLayout();
    QLabel *label = new QLabel(message);
    layout->addWidget(label);
    layout->setAlignment(label, Qt::AlignCenter);

    widget->setLayout(layout);

    return widget;
}

QActionGroup *WidgetHelper::addSplitDirectionActions(QWidget *w)
{
    QActionGroup *splitDirectionGroup=new QActionGroup(w);

    QAction *hoizontal = new QAction(IconUtil::getIcon("vertical"), QObject::tr("Horizontal layout"), splitDirectionGroup);
    hoizontal->setCheckable(true);
    hoizontal->setData(Qt::Horizontal);
    hoizontal->setChecked(true);

    QAction *vertical = new QAction(IconUtil::getIcon("horizontal"), QObject::tr("Vertical layout"), splitDirectionGroup);
    vertical->setCheckable(true);
    vertical->setData(Qt::Vertical);

    w->addActions(splitDirectionGroup->actions());

    return splitDirectionGroup;
}

void WidgetHelper::createAutoRefreshComboBox(QToolBar *toolbar, QWidget *receiver)
{
    //toolbar->addWidget(new QLabel(QObject::tr("Refresh: ")));

    AutoRefreshComboBox *comboBox = new AutoRefreshComboBox(receiver);
    comboBox->setToolTip(QObject::tr("Auto refresh interval"));
    toolbar->addWidget(comboBox);
}


