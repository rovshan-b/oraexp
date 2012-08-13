#include "codeeditorandsearchpanewidget.h"
#include <QtGui>

CodeEditorAndSearchPaneWidget::CodeEditorAndSearchPaneWidget(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *layout=new QVBoxLayout();

    codeEditor=new CodeEditor();
    searchPane=new SearchPane(codeEditor);

    codeEditor->installEventFilter(this);
    searchPane->getFindTextBox()->installEventFilter(this);
    searchPane->getReplaceTextBox()->installEventFilter(this);

    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    layout->addWidget(codeEditor);
    layout->addWidget(searchPane);
    searchPane->setVisible(false);

    setLayout(layout);

    connect(codeEditor, SIGNAL(escapeKeyPressed()), this, SLOT(hideSearchPane()));
}

void CodeEditorAndSearchPaneWidget::keyReleaseEvent ( QKeyEvent * event )
{
    if(event->key()==Qt::Key_Escape){
        hideSearchPane();
    }else{
        QWidget::keyReleaseEvent(event);
    }
}

bool CodeEditorAndSearchPaneWidget::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type()==QEvent::FocusIn){
        emit focusEvent(this, true);
    }

    return QWidget::eventFilter(obj, event);
}

void CodeEditorAndSearchPaneWidget::hideSearchPane()
{
    bool visible = searchPane->isVisible();
    if(visible){
        searchPane->closePane();
    }

    codeEditor->setFocus();

    if(!visible){
        emit escapeKeyPressed();
    }
}

void CodeEditorAndSearchPaneWidget::showSearchPane()
{
    if(!isEnabled()){
        return;
    }

    if(!searchPane->isVisible()){
        searchPane->show();
        searchPane->searchTextChanged();
    }

    searchPane->activateFindTextBox();
}
