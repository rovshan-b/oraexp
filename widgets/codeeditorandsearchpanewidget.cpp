#include "codeeditorandsearchpanewidget.h"
#include <QtGui>

CodeEditorAndSearchPaneWidget::CodeEditorAndSearchPaneWidget(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *layout=new QVBoxLayout();

    codeEditor=new CodeEditor();
    searchPane=new SearchPane(codeEditor);

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
    QWidget::keyReleaseEvent(event);

    if(event->key()==Qt::Key_Escape){
        hideSearchPane();
    }
}

void CodeEditorAndSearchPaneWidget::hideSearchPane()
{
    if(searchPane->isVisible()){
        searchPane->closePane();
    }

    codeEditor->setFocus();
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
