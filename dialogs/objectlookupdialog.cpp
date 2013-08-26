#include "objectlookupdialog.h"
#include "util/dialoghelper.h"
#include "util/queryutil.h"
#include "util/widgethelper.h"
#include "connectivity/dbconnection.h"
#include "widgets/datatable.h"
#include <QtGui>
#include <QDebug>

ObjectLookupDialog::ObjectLookupDialog(DbUiManager *uiManager, QWidget *parent) :
    ConnectionPageWindow(uiManager, parent),
    timerId(-1),
    searchInProgress(false)
{
    new QShortcut(QKeySequence("Ctrl+K"), this, SLOT(focusSearchBox()));
}

void ObjectLookupDialog::createUi()
{
    setAttribute(Qt::WA_DeleteOnClose, false);
    setWindowFlags(Qt::FramelessWindowHint|Qt::Popup);
    setBackgroundRole(QPalette::Window);
    //setAutoFillBackground(true);
    //setBackgroundRole(QPalette::Base);
    //setModal(true);

    //setAutoFillBackground(false);
    //setAttribute(Qt::WA_TranslucentBackground, true);

//    setStyleSheet( //"padding: 0px;"
//                   "border-style: solid;"
//                   "border-color: gray;"
//                   "border-width: 1px;"
//                   "border-radius: 2px;"
//                   );

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(1,1,1,1);
    mainLayout->setSpacing(0);

    objectNameEditor = new QLineEdit();
    objectNameEditor->setFrame(false);
    //WidgetHelper::changeFontSize(objectNameEditor, -1);
    mainLayout->addWidget(objectNameEditor);
    mainLayout->setAlignment(objectNameEditor, Qt::AlignTop);

    table = new DataTable();
    table->setFrameStyle(QFrame::NoFrame);
    table->setUiManager(uiManager);
    table->verticalHeader()->hide();
    table->horizontalHeader()->hide();
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setGridStyle(Qt::NoPen);
    table->setModel(new QStandardItemModel(0, 3, this));
    table->hideColumn(2);
    table->hideColumn(1);
    table->horizontalHeader()->setStretchLastSection(true);
    table->setContentsMargins(0,0,0,0);
    table->setObjectListMode(1, 0, -1, 2);
    table->setIconColumn("OBJECT_NAME", "ICON_NAME");
    table->setVisible(false);
    WidgetHelper::changeFontSize(table, -1);
    mainLayout->addWidget(table);

    setLayout(mainLayout);

    objectNameEditor->setFocus();

    connect(objectNameEditor, SIGNAL(textEdited(QString)), this, SLOT(textEdited(QString)));
    connect(table, SIGNAL(firstFetchCompleted()), this, SLOT(firstFetchCompleted()));
    connect(table, SIGNAL(contextMenuTriggered(QAction*)), this, SLOT(hide()));
    connect(table, SIGNAL(activated(QModelIndex)), this, SLOT(indexActivated(QModelIndex)));

    QSize hint = sizeHint();
    resize(qMax(hint.width(), 300), hint.height());
    DialogHelper::centerWindow(this);

    objectNameEditor->installEventFilter(this);
    table->installEventFilter(this);
}

void ObjectLookupDialog::closeEvent(QCloseEvent *event)
{
    stopTimer();

    event->accept();
}

void ObjectLookupDialog::focusOutEvent(QFocusEvent *event)
{
    Q_UNUSED(event);

    close();
}

void ObjectLookupDialog::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);

    if(timerId == -1){
        timerId = startTimer(1000);
        focusSearchBox();
    }
}

void ObjectLookupDialog::hideEvent(QHideEvent *event)
{
    Q_UNUSED(event);

    stopTimer();
}

void ObjectLookupDialog::timerEvent(QTimerEvent *event)
{
    if(event->timerId() != timerId
            || searchInProgress
            || this->getDb() == 0
            || this->isBusy()){
        return;
    }

    if(lastText != objectNameEditor->text()){
        lastText = objectNameEditor->text();

        if(lastText.isEmpty()){
            return;
        }

        searchInProgress = true;

        QString objName = QString("%1%").arg(lastText.toUpper()).replace("_", "\\_");
        table->displayQueryResults(this,
                                   QueryUtil::getQuery("get_object_lookup_results", this->getDb()),
                                   QList<Param*>()
                                   << new Param("owner", this->getDb()->getSchemaName())
                                   << new Param("object_name", objName));
    }
}

bool ObjectLookupDialog::eventFilter(QObject *obj, QEvent *event)
{
    if((obj==objectNameEditor || obj==table) &&
            event->type() == QEvent::KeyPress &&
            table->isVisible()){
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if(obj==objectNameEditor && keyEvent->key() == Qt::Key_Down){
            table->setFocus();

            QModelIndex index = table->model()->index(0,0);
            table->setCurrentIndex(index);
            table->selectionModel()->select(index, QItemSelectionModel::SelectCurrent);
            table->scrollTo(index, QAbstractItemView::EnsureVisible);

            return true;
        }else if(obj==table && keyEvent->key() == Qt::Key_Up){
            if(table->currentIndex().row()==0){
                focusSearchBox();
                return true;
            }
        }
    }

    return ConnectionPageWindow::eventFilter(obj, event);
}

void ObjectLookupDialog::textEdited(const QString &newText)
{
    Q_UNUSED(newText);
}

void ObjectLookupDialog::firstFetchCompleted()
{
    int rowCount = table->model()->rowCount();
    table->setVisible(rowCount > 0);

    QPropertyAnimation *animation = new QPropertyAnimation(this, "size");
    animation->setDuration(200);
    animation->setStartValue(this->size());
    int newHeight;

    if(rowCount == 0){
        newHeight = this->sizeHint().height();
    }else{
        newHeight = objectNameEditor->height();
        newHeight += table->verticalHeader()->defaultSectionSize() * qMin(10, rowCount) + 4;
    }

    animation->setEndValue(QSize(this->width(), newHeight));

    animation->start(QPropertyAnimation::DeleteWhenStopped);

    this->searchInProgress = false;
}

void ObjectLookupDialog::focusSearchBox()
{
    objectNameEditor->setFocus();
    objectNameEditor->selectAll();
}

void ObjectLookupDialog::indexActivated(const QModelIndex &index)
{
    if(index.isValid()){
        this->hide();
        table->invokeDefaultActionForObject(index.row());
    }
}

void ObjectLookupDialog::stopTimer()
{
    if(timerId!=-1){
        killTimer(timerId);
        timerId = -1;
    }
}
