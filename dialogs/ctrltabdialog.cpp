#include "ctrltabdialog.h"
#include "beans/ctrltabdata.h"
#include <QtGui>

CtrlTabDialog::CtrlTabDialog(const QList<CtrlTabData *> &data, QWidget *parent) :
    QDialog(parent)
{
    Q_ASSERT(data.size()>0);

    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(1,1,1,1);

    listView = new QListView();
    listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    listModel = new QStandardItemModel(this);
    listView->setModel(listModel);
    populateList(data);
    listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mainLayout->addWidget(listView);

    setLayout(mainLayout);

    listView->installEventFilter(this);

    connect(listView, SIGNAL(activated(QModelIndex)), this, SLOT(listItemActivated(QModelIndex)));
}

QWidget *CtrlTabDialog::getSelectedWidget() const
{
    int ix = listView->currentIndex().row();
    if(ix==-1){
        return 0;
    }

    QStandardItem *item = listModel->item(ix);
    return item->data().value<QWidget*>();
}

bool CtrlTabDialog::eventFilter(QObject *obj, QEvent *event)
{
    if(obj==listView &&
            (event->type()==QEvent::KeyPress || event->type()==QEvent::KeyRelease)){
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        return handleKeyEvent(keyEvent, event->type()==QEvent::KeyPress);
    }

    return false;
}

void CtrlTabDialog::keyPressEvent(QKeyEvent *event)
{
    if(!handleKeyEvent(event, true)){
        QDialog::keyPressEvent(event);
    }
}

void CtrlTabDialog::keyReleaseEvent(QKeyEvent *event)
{
    if(!handleKeyEvent(event, false)){
        QDialog::keyReleaseEvent(event);
    }
}

void CtrlTabDialog::listItemActivated(const QModelIndex &index)
{
    Q_UNUSED(index);
    this->accept();
}

void CtrlTabDialog::populateList(const QList<CtrlTabData *> &data)
{
    foreach(CtrlTabData *item, data){
        QStandardItem *row = new QStandardItem(item->icon, item->title);
        row->setData(QVariant::fromValue(item->widget));
        listModel->appendRow(row);
    }

    qDeleteAll(data);

    listView->setCurrentIndex(listModel->index(1,0));
}

bool CtrlTabDialog::handleKeyEvent(QKeyEvent *keyEvent, bool keyPress)
{
    if(!keyPress && keyEvent->key()==Qt::Key_Control){
        keyEvent->accept();
        this->accept();

        return true;
    }else if(keyPress && keyEvent->key()==Qt::Key_Tab){
        keyEvent->accept();

        move((keyEvent->modifiers() & Qt::ShiftModifier)==0);

        return true;
    }

    return false;
}

void CtrlTabDialog::move(bool next)
{
    Q_UNUSED(next);

    int ix = listView->currentIndex().row();
    int newIx = (ix<listModel->rowCount()-1) ? ix+1 : 0;
    listView->setCurrentIndex(listModel->index(newIx, 0));
}
