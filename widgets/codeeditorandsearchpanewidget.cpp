#include "codeeditorandsearchpanewidget.h"
#include <QtGui>

CodeEditorAndSearchPaneWidget::CodeEditorAndSearchPaneWidget(bool enableCodeCollapsing, QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *layout=new QVBoxLayout();

    codeEditor=new CodeEditor(enableCodeCollapsing);
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

void CodeEditorAndSearchPaneWidget::setTextDocument(QTextDocument *document) const
{
    qDebug() << "setting document";
    editor()->setDocument(document);
}

QTextDocument *CodeEditorAndSearchPaneWidget::getTextDocument() const
{
    return editor()->document();
}

void CodeEditorAndSearchPaneWidget::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    this->codeEditor->setQueryScheduler(queryScheduler);
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
    bool visible = closeSearchPane();

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

    QString selectedText = codeEditor->textCursor().selectedText();
    bool hasSelection = selectedText.size() > 1 && selectedText.size() <= 150;
    if(hasSelection){
        searchPane->setSearchText(selectedText);
    }

    if(!searchPane->isVisible()){
        searchPane->show();
    }

    searchPane->searchTextChanged(!hasSelection);
    searchPane->activateFindTextBox();

    if(hasSelection){
        QTextCursor cur = codeEditor->textCursor();
        int startPos = qMin(cur.selectionStart(), cur.selectionEnd());
        cur.setPosition(startPos);
        searchPane->findNextOrPrevious(true, cur);
    }
}

void CodeEditorAndSearchPaneWidget::setReadOnly(bool readOnly)
{
    this->codeEditor->setReadOnly(readOnly);
    this->searchPane->setReadOnly(readOnly);
}

void CodeEditorAndSearchPaneWidget::setInitialText(const QString &text)
{
    this->codeEditor->setPlainText(text);
    this->codeEditor->document()->setModified(false);
}

bool CodeEditorAndSearchPaneWidget::closeSearchPane()
{
    bool visible = searchPane->isVisible();

    if(visible){
        searchPane->closePane();
    }

    return visible;
}
