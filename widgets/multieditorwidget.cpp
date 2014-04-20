#include "multieditorwidget.h"
#include "util/iconutil.h"
#include "util/widgethelper.h"
#include "code_parser/plsql/plsqlscanner.h"
#include "code_parser/plsql/plsqlparsehelper.h"
#include "code_parser/plsql/plsqlparsingtable.h"
#include "beans/codecollapseposition.h"
#include "util/strutil.h"
#include <QtGui>

MultiEditorWidget::MultiEditorWidget(bool enableCodeCollapsing, QWidget *parent) :
    QWidget(parent),
    currentEditor(0),
    splitDirectionGroup(0),
    infoLabel(0),
    enableCodeCollapsing(enableCodeCollapsing),
    timerId(-1)
{
    createUi();
}

MultiEditorWidget::~MultiEditorWidget()
{
    //qDeleteAll(collapsePositions);
    //collapsePositions.clear();
}

void MultiEditorWidget::createUi()
{
    QVBoxLayout *mainLayout=new QVBoxLayout();
    mainLayout->setContentsMargins(0,0,0,0);

    editorSplitter = new QSplitter(Qt::Horizontal);
    editorSplitter->setChildrenCollapsible(false);
    CodeEditorAndSearchPaneWidget *firstEditor = createEditor();
    editorSplitter->addWidget(firstEditor);

    mainLayout->addWidget(editorSplitter);

    setLayout(mainLayout);

    connect(firstEditor->editor()->document(), SIGNAL(contentsChange(int,int,int)), this, SLOT(documentContentsChanged(int,int,int)));
}

CodeEditorAndSearchPaneWidget *MultiEditorWidget::getCurrentEditor() const
{
    Q_ASSERT(currentEditor);
    return currentEditor;
}

void MultiEditorWidget::addSplittingActions(QWidget *w)
{
    Q_ASSERT(splitDirectionGroup==0);

    QActionGroup *editorCountGroup=new QActionGroup(this);
    for(int i=1; i<=3; ++i){
        QAction *action=new QAction(QString::number(i), editorCountGroup);
        action->setCheckable(true);
        action->setData(i);

        if(i==1){
            action->setChecked(true);
            action->setToolTip(tr("Single editor pane"));
        }else{
            action->setToolTip(tr("Split editor to %1 panes").arg(i));
        }

        connect(action, SIGNAL(triggered(bool)), this, SLOT(editorCountActionSelected(bool)));
    }
    w->addActions(editorCountGroup->actions());

    QAction *separator=new QAction(w);
    separator->setSeparator(true);

    splitDirectionGroup = WidgetHelper::addSplitDirectionActions(w);

    splitDirectionGroup->setEnabled(false);
    connect(splitDirectionGroup, SIGNAL(triggered(QAction*)), this, SLOT(editorOrientationActionSelected(QAction*)));
}

QLabel *MultiEditorWidget::createInfoLabel()
{
    Q_ASSERT(infoLabel==0);

    infoLabelTextFormat=QString("Line:%1 Pos:%2 (%3)");
    infoLabel=new QLabel();
    infoLabel->setText(infoLabelTextFormat);
    //infoLabel->setMinimumWidth(infoLabel->fontMetrics().width(infoLabelTextFormat)*1.5);

    return infoLabel;
}

void MultiEditorWidget::editorCountActionSelected(bool checked)
{
    if(checked){
        QAction *action=static_cast<QAction*>(sender());
        Q_ASSERT(action);

        setEditorCount(action->data().toInt());
    }
}

void MultiEditorWidget::editorOrientationActionSelected(QAction *action)
{
    if(!action->isChecked()){
        return;
    }

    editorSplitter->setOrientation((Qt::Orientation)action->data().toInt());
}

void MultiEditorWidget::codeEditorFocusEvent(QWidget *object, bool focusIn)
{
    currentEditor = qobject_cast<CodeEditorAndSearchPaneWidget*>(object);
    Q_ASSERT(currentEditor);
    cursorPositionChanged();

    /*
    if(enableCodeCollapsing){
        if(focusIn){
            timerId = startTimer(500);
        }else{
            killTimer(timerId);
        }
    }*/
}

void MultiEditorWidget::setEditorCount(int count)
{
    Q_ASSERT(count>0);

    int currentCount = visibleEditorCount();
    if(count>currentCount){
        for(int i=currentCount; i<=count; ++i){
            if(editors.size()>=i){
                editors.at(i-1)->show();
            }else{
                editorSplitter->addWidget(createEditor());
            }
        }
    }else if(count<currentCount){
        for(int i=currentCount; i>count; --i){
            if(editors.at(i-1)==currentEditor){
                Q_ASSERT(i-2 >= 0);
                editors.at(i-2)->editor()->setFocus();
            }

            editors.at(i-1)->closeSearchPane();
            editors.at(i-1)->hide();
        }
    }

    splitDirectionGroup->setEnabled(count>1);
}

CodeEditorAndSearchPaneWidget *MultiEditorWidget::createEditor()
{
    CodeEditorAndSearchPaneWidget *editor=new CodeEditorAndSearchPaneWidget(this->enableCodeCollapsing);
    connect(editor, SIGNAL(escapeKeyPressed()), this, SIGNAL(escapeKeyPressed()));
    editors.append(editor);

    if(editors.size()>1){
        editor->editor()->setDocument(editors.at(0)->editor()->document()); //bind all instances to the same document (first created one)
    }

    if(editors.size()==1){
        currentEditor=editor;
    }

    /*
    if(enableCodeCollapsing){

        editor->editor()->setCodeCollapsePositions(&this->collapsePositions);

        if(editors.size() == 1){
            connect(editor->editor()->document(), SIGNAL(contentsChanged()), this, SLOT(documentChanged()));
            //connect(editor->editor()->document(), SIGNAL(blockCountChanged(int)), this, SLOT(recalculateCollapsePositions()));
        }
    }*/

    connect(editor, SIGNAL(focusEvent(QWidget*,bool)), this, SLOT(codeEditorFocusEvent(QWidget*,bool)));
    connect(editor->editor(), SIGNAL(cursorPositionChanged()), this, SLOT(cursorPositionChanged()));

    return editor;
}

void MultiEditorWidget::setReadOnly(bool readOnly)
{
    foreach(CodeEditorAndSearchPaneWidget *editor, editors){
        editor->setReadOnly(readOnly);
    }
}

void MultiEditorWidget::setInfoLabelTextFormat(const QString &format)
{
    this->infoLabelTextFormat=format;
}

QList<CodeEditorAndSearchPaneWidget *> MultiEditorWidget::getEditors() const
{
    return this->editors;
}

int MultiEditorWidget::visibleEditorCount() const
{
    int result=0;

    for(int i=0; i<editors.size(); ++i){
        if(editors.at(i)->isVisible()){
            ++result;
        }
    }

    return result;
}

void MultiEditorWidget::cursorPositionChanged()
{
    if(infoLabel==0){
        return;
    }

    QTextCursor cursor=currentEditor->editor()->textCursor();
    int block=cursor.blockNumber();
    int posInBlock=cursor.positionInBlock();
    int pos=cursor.position();

    infoLabel->setText(infoLabelTextFormat.arg(QString::number(block+1), QString::number(posInBlock+1), QString::number(pos+1)));
}

void MultiEditorWidget::documentContentsChanged(int position, int charsRemoved, int charsAdded)
{
    lastChangeTime = QTime::currentTime();
}

/*
void MultiEditorWidget::timerEvent(QTimerEvent *event)
{
    if(event->timerId() != timerId){
        return;
    }

    if(!lastChangeTime.isNull() && lastChangeTime.msecsTo(QTime::currentTime())>=1000){
        recalculateCollapsePositions();
        lastChangeTime=QTime();
    }
}

void MultiEditorWidget::recalculateCollapsePositions()
{
    qDeleteAll(collapsePositions);

    QTextCursor cur = currentEditor->editor()->textCursor();
    cur.setPosition(0);
    collapsePositions = PlSqlParseHelper::findCodeCollapsePositions(cur);

    for(int i=0; i<editors.size(); ++i){
        editors.at(i)->editor()->refreshCodeCollapseArea();
    }
}
*/
