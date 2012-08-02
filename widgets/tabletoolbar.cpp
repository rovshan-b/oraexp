#include "tabletoolbar.h"
#include "widgets/datatable.h"
#include "util/iconutil.h"
#include "models/genericeditabletablemodel.h"
#include <QtGui>

TableToolbar::TableToolbar(DataTable *table, QWidget *parent) :
    QToolBar(parent), table(table)
{
    setIconSize(QSize(16, 16));

    addMoreRowsAction=new QAction(IconUtil::getIcon("add"), tr("Add new row"), this);
    addActionMenu = new QMenu(tr("Add copy of row"), this);
    addMoreRowsAction->setMenu(addActionMenu);

    insertRowAction=new QAction(IconUtil::getIcon("insert_row"), tr("Insert row at current position"), this);
    deleteCurrentRowAction=new QAction(IconUtil::getIcon("delete"), tr("Delete selected rows"), this);

    moveUpAction=new QAction(IconUtil::getIcon("move_up"), tr("Move up"), this);
    moveDownAction=new QAction(IconUtil::getIcon("move_down"), tr("Move down"), this);

    addAction(addMoreRowsAction);
    addAction(insertRowAction);
    addAction(deleteCurrentRowAction);

    addSeparator();

    addAction(moveUpAction);
    addAction(moveDownAction);

    connect(addMoreRowsAction, SIGNAL(triggered()), this, SLOT(addMoreRows()));
    connect(addActionMenu, SIGNAL(aboutToShow()), this, SLOT(populateAddActionMenu()));

    connect(insertRowAction, SIGNAL(triggered()), this, SLOT(insertRow()));
    connect(deleteCurrentRowAction, SIGNAL(triggered()), this, SLOT(deleteCurrentRow()));

    connect(moveUpAction, SIGNAL(triggered()), this, SLOT(moveRowUp()));
    connect(moveDownAction, SIGNAL(triggered()), this, SLOT(moveRowDown()));
}

void TableToolbar::addMoreRows()
{
    QAbstractItemModel *model=table->model();
    model->insertRows(model->rowCount(), 1);
}

void TableToolbar::addCopyOf()
{
    QAction *action=qobject_cast<QAction*>(sender());
    Q_ASSERT(action);

    QVariant var=action->data();
    if(!var.isValid()){
        return;
    }

    GenericEditableTableModel *model=qobject_cast<GenericEditableTableModel*>(table->model());
    Q_ASSERT(model);

    model->addCopyOf(var.toInt());
}

void TableToolbar::insertRow()
{
    QItemSelectionModel *selection=table->selectionModel();
    QModelIndex currentIndex=selection->currentIndex();
    int rowToInsert=0;

    if(currentIndex.isValid()){
       rowToInsert=currentIndex.row();
    }

    GenericEditableTableModel *model=qobject_cast<GenericEditableTableModel*>(table->model());
    Q_ASSERT(model);

    if(rowToInsert>model->getFrozenRow()){
        model->insertRows(rowToInsert, 1);
    }else{
        QMessageBox::information(this->window(), tr("Not a valid position"),
                                 tr("Cannot insert row at this position"));
    }
}

void TableToolbar::deleteCurrentRow()
{
    QModelIndex currentIndex=table->currentIndex();
    if(currentIndex.isValid()){

        int row=currentIndex.row();
        int prevRowIndex=row>0 ? row-1 : 0;
        int colIndex=currentIndex.column();

        QItemSelectionModel *selModel=table->selectionModel();
        Q_ASSERT(selModel);

        GenericEditableTableModel *model=qobject_cast<GenericEditableTableModel*>(table->model());
        Q_ASSERT(model);

        bool prompted=false;

        int rowCount=model->rowCount();
        for(int i=rowCount-1; i>=0; --i){
            if(selModel->rowIntersectsSelection(i, QModelIndex()) || i==row){

                if(!prompted && QMessageBox::question(this->window(),
                                                      tr("Confirm deletion"),
                                                      tr("Delete selected rows?"),
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

        selectRowAfterDeletion(model, prevRowIndex, colIndex);
    }
}

void TableToolbar::selectRowAfterDeletion(GenericEditableTableModel *model, int prevRowIndex, int colIndex)
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

void TableToolbar::moveRowUp()
{
    moveRows(true);
}

void TableToolbar::moveRowDown()
{
    moveRows(false);
}

void TableToolbar::moveRows(bool moveUp) const
{
    QItemSelectionModel *selectionModel=table->selectionModel();
    Q_ASSERT(selectionModel);

    QModelIndex index=selectionModel->currentIndex();
    if(!index.isValid()){
        QMessageBox::information(this->window(), tr("No selection"),
                                 tr("Please, select one or more rows to move"));
        return;
    }

    GenericEditableTableModel *model=qobject_cast<GenericEditableTableModel*>(table->model());
    Q_ASSERT(model);


    //collect rows that will be moved
    QList<int> rowsToMove;
    for(int i=0; i<model->rowCount(); ++i){
        if((selectionModel->rowIntersectsSelection(i, QModelIndex()) || i==index.row())
                && i>model->getFrozenRow()){
            rowsToMove.append(i);
        }
    }

    QModelIndex indexToSetAsCurrent;
    selectionModel->clearSelection();
    if(moveUp){
        for(int i=0; i<model->rowCount(); ++i){
            if(i>0 && i>model->getFrozenRow()+1 && rowsToMove.contains(i) && !rowsToMove.contains(i-1)){
                model->moveRowUp(i);
                rowsToMove.removeOne(i);
                selectionModel->select(model->index(i-1, 0), QItemSelectionModel::Rows | QItemSelectionModel::Select);

                if(!indexToSetAsCurrent.isValid()){
                    indexToSetAsCurrent=model->index(i-1, index.column());
                }
            }
        }
    }else{
        for(int i=model->rowCount()-1; i>=0; --i){
            if(i<model->rowCount()-2 && rowsToMove.contains(i) && !rowsToMove.contains(i+1)){
                model->moveRowDown(i);
                rowsToMove.removeOne(i);
                selectionModel->select(model->index(i+1, 0), QItemSelectionModel::Rows | QItemSelectionModel::Select);

                indexToSetAsCurrent=model->index(i+1, index.column());
            }
        }
    }

    if(indexToSetAsCurrent.isValid()){
        selectionModel->setCurrentIndex(indexToSetAsCurrent, QItemSelectionModel::Rows | QItemSelectionModel::Select);
    }
}

void TableToolbar::populateAddActionMenu()
{
    qDeleteAll(addActionMenu->actions());

    GenericEditableTableModel *model=qobject_cast<GenericEditableTableModel*>(table->model());
    QAction *action;
    QString title;
    for(int i=0; i<model->rowCount(); ++i){
        title=model->data(model->index(i, model->getTitleColumn())).toString();
        if(model->isRowDataCorrect(i).isEmpty() && !title.isEmpty()){
            action=new QAction(title, this);
            action->setIcon(model->headerData(i, Qt::Vertical, Qt::DecorationRole).value<QPixmap>());
            action->setData(i);
            connect(action, SIGNAL(triggered()), this, SLOT(addCopyOf()));
            addActionMenu->addAction(action);
        }
    }

    if(addActionMenu->actions().isEmpty()){
        addActionMenu->addAction(tr("no item to copy"))->setEnabled(false);
    }
}
