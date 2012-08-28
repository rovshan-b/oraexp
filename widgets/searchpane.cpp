#include "searchpane.h"
#include "widgets/closebutton.h"
#include "codeeditor/codeeditor.h"
#include "util/iconutil.h"
#include <QtGui>

SearchPane::SearchPane(CodeEditor *editor, QWidget *parent) :
    QWidget(parent), editor(editor)
{   

    QHBoxLayout *mainLayout=new QHBoxLayout();
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(2);

    QFormLayout *form=new QFormLayout();
    form->setContentsMargins(0,2,0,0);
    form->setSpacing(2);

    findTextBox=new QLineEdit();
    form->addRow(tr("Find:"), findTextBox);

    replaceTextBox=new QLineEdit();
    form->addRow(tr("Replace with:"), replaceTextBox);

    mainLayout->addLayout(form);

    mainLayout->addLayout(createButtons());
   // mainLayout->setAlignment(buttonsLayout, Qt::AlignTop);

    QVBoxLayout *rightLayout=new QVBoxLayout();
    rightLayout->setContentsMargins(0,0,0,0);
    rightLayout->setSpacing(2);

    CloseButton *btnClose=new CloseButton();
    rightLayout->addWidget(btnClose);
    connect(btnClose, SIGNAL(clicked()), this, SLOT(closePane()));

    mainLayout->addStretch();
    mainLayout->addLayout(rightLayout);
    mainLayout->setAlignment(rightLayout, Qt::AlignHCenter|Qt::AlignTop);

    setLayout(mainLayout);

    connect(findTextBox, SIGNAL(textChanged(QString)), this, SLOT(searchTextChanged()));
    connect(findTextBox, SIGNAL(returnPressed()), this, SLOT(findNext()));
    connect(replaceTextBox, SIGNAL(returnPressed()), this, SLOT(replaceAndFind()));
    connect(findPreviousButton, SIGNAL(clicked()), this, SLOT(findPrevious()));
    connect(findNextButton, SIGNAL(clicked()), this, SLOT(findNext()));

    connect(replaceOneButton, SIGNAL(clicked()), this, SLOT(replaceOne()));
    connect(replaceAndFindButton, SIGNAL(clicked()), this, SLOT(replaceAndFind()));
    connect(replaceAllButton, SIGNAL(clicked()), this, SLOT(replaceAll()));

    findTextBox->installEventFilter(this);
}

QLayout *SearchPane::createButtons()
{
    QVBoxLayout *buttonsLayout=new QVBoxLayout();

    QHBoxLayout *findPrevNextLayout=new QHBoxLayout();
    findPrevNextLayout->setContentsMargins(0,2,0,0);

    findPreviousButton=new QToolButton(this);
    findPreviousButton->setText(tr("Find previous"));
    findPreviousButton->setToolTip(tr("Find previous"));
    findPreviousButton->setIcon(IconUtil::getIcon("previous"));
    findPreviousButton->setAutoRaise(true);
    findPrevNextLayout->addWidget(findPreviousButton);
    findPrevNextLayout->setAlignment(findPreviousButton, Qt::AlignLeft);

    findNextButton=new QToolButton(this);
    findNextButton->setText(tr("Find next"));
    findNextButton->setToolTip(tr("Find next"));
    findNextButton->setIcon(IconUtil::getIcon("next"));
    findNextButton->setAutoRaise(true);
    findPrevNextLayout->addWidget(findNextButton);
    findPrevNextLayout->setAlignment(findNextButton, Qt::AlignLeft);

    buttonsLayout->addLayout(findPrevNextLayout);
    buttonsLayout->setAlignment(findPrevNextLayout, Qt::AlignLeft);

    QHBoxLayout *replaceButtonsLayout=new QHBoxLayout();

    replaceOneButton=new QToolButton(this);
    replaceOneButton->setText(tr("Replace"));
    replaceOneButton->setToolTip(tr("Replace"));
    replaceOneButton->setAutoRaise(true);
    replaceOneButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
    replaceButtonsLayout->addWidget(replaceOneButton);

    replaceAndFindButton=new QToolButton(this);
    replaceAndFindButton->setText(tr("Replace && Find"));
    replaceAndFindButton->setToolTip(tr("Replace & Find next occurence"));
    replaceAndFindButton->setAutoRaise(true);
    replaceAndFindButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
    replaceButtonsLayout->addWidget(replaceAndFindButton);

    replaceAllButton=new QToolButton(this);
    replaceAllButton->setText(tr("Replace all"));
    replaceAllButton->setToolTip(tr("Replace all"));
    replaceAllButton->setAutoRaise(true);
    replaceAllButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
    replaceButtonsLayout->addWidget(replaceAllButton);

    buttonsLayout->addLayout(replaceButtonsLayout);

    return buttonsLayout;
}

void SearchPane::activateFindTextBox()
{
    findTextBox->setFocus();
    findTextBox->selectAll();
    //findPrevious();
}

void SearchPane::searchTextChanged(bool setSelection)
{
    QString searchText=findTextBox->text();
    if(searchText.isEmpty()){
        editor->clearFoundTextPositions();
        return;
    }

    QTextDocument *doc=editor->document();
    //int currentPosInDoc=editor->textCursor().position();

    QList< QTextCursor > searchTextOccurences;
    int searchStartPos=0;
    QTextCursor foundCursor=doc->find(searchText, searchStartPos);
    QTextCursor firstMatch=foundCursor;

    //bool foundFirstMatchAfterCurrentPosition=false;
    while(!foundCursor.isNull()){
        searchTextOccurences.append(foundCursor);
        searchStartPos=foundCursor.selectionEnd();
        foundCursor=doc->find(searchText, searchStartPos);

        //if(!foundFirstMatchAfterCurrentPosition && !foundCursor.isNull() && foundCursor.position()>currentPosInDoc){
        //    qDebug() << "changing first match,currentPosInDoc="<<currentPosInDoc<<"foundCursor.position()="<<foundCursor.position();
        //    firstMatch=foundCursor;
        //    foundFirstMatchAfterCurrentPosition=true;
        //}
    }

    if(searchTextOccurences.count()>0){
        editor->setFoundTextPositions(searchTextOccurences);
    }else{
        editor->clearFoundTextPositions();
    }

    if(!firstMatch.isNull() && setSelection){
        editor->setTextCursor(firstMatch);
        editor->centerCursor();
    }
}

QTextCursor SearchPane::findNextOrPrevious(bool next, const QTextCursor &baseCursor)
{
    QString searchText=findTextBox->text();
    if(searchText.isEmpty()){
        return QTextCursor();
    }

    QTextDocument *doc=editor->document();
    QTextCursor foundCursor=doc->find(searchText, baseCursor, next ? (QTextDocument::FindFlag)0 : QTextDocument::FindBackward);
    if(!foundCursor.isNull()){
        editor->setTextCursor(foundCursor);
        return foundCursor;
    }else{
        //search once again from start/end
        QTextCursor cursor=baseCursor;
        if(next){
            cursor.setPosition(0);
        }else{
            cursor.movePosition(QTextCursor::End);
        }
        foundCursor=doc->find(searchText, cursor, next ? (QTextDocument::FindFlag)0 : QTextDocument::FindBackward);
        if(!foundCursor.isNull()){
            editor->setTextCursor(foundCursor);
            return foundCursor;
        }
    }

    return QTextCursor();
}

bool SearchPane::findNext()
{
    return !findNextOrPrevious(true, editor->textCursor()).isNull();
}

bool SearchPane::findPrevious()
{
    return !findNextOrPrevious(false, editor->textCursor()).isNull();
}

void SearchPane::replace(bool highlightNext)
{
    if(isReadOnly()){
        return;
    }

    QString searchText=findTextBox->text();
    QString textReplaceWith=replaceTextBox->text();
    if(searchText.isEmpty() || textReplaceWith==findTextBox->text()){
        return;
    }

    editor->clearFoundTextPositions();
    editor->setUpdatesEnabled(false);


    QTextCursor currentCursor=editor->textCursor();
    currentCursor.setPosition(qMin(currentCursor.selectionStart(), currentCursor.selectionEnd()));

    QTextCursor foundCursor=findNextOrPrevious(true, currentCursor);
    if(!foundCursor.isNull()){
        int positionBeforeInsert=qMin(foundCursor.selectionStart(), foundCursor.selectionEnd());
        foundCursor.insertText(textReplaceWith);
        foundCursor.setPosition(positionBeforeInsert);
        foundCursor.setPosition(positionBeforeInsert+textReplaceWith.length(), QTextCursor::KeepAnchor);

        searchTextChanged(highlightNext);

        if(!highlightNext){
            editor->setTextCursor(foundCursor);
        }
    }

    editor->setUpdatesEnabled(true);
    //searchTextChanged();
}

void SearchPane::replaceOne()
{
    replace(false);
}

void SearchPane::replaceAndFind()
{
    replace(true);
}

void SearchPane::replaceAll()
{
    if(isReadOnly()){
        return;
    }

    QString searchText=findTextBox->text();
    QString textReplaceWith=replaceTextBox->text();
    if(searchText.isEmpty() || textReplaceWith==findTextBox->text()){
        return;
    }

    editor->clearFoundTextPositions();
    editor->setUpdatesEnabled(false);

    QTextCursor currentCursor=editor->textCursor();
    currentCursor.setPosition(0);
    currentCursor.beginEditBlock();

    QTextDocument *doc=editor->document();
    QTextCursor foundCursor;

    int lastPos=0;
    while(true){
        foundCursor=doc->find(searchText, lastPos);
        if(!foundCursor.isNull()){
            foundCursor.insertText(textReplaceWith);
            lastPos=foundCursor.position();
        }else{
            break;
        }
    }

    currentCursor.endEditBlock();

    editor->setUpdatesEnabled(true);
}

void SearchPane::closePane()
{
    this->hide();
    editor->clearFoundTextPositions();
}

bool SearchPane::eventFilter(QObject *obj, QEvent *event)
 {
     if (event->type() == QEvent::KeyRelease && obj==findTextBox) {
         QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

         if(keyEvent->key()==Qt::Key_Up){
             findPrevious();
             return true;
         }else if(keyEvent->key()==Qt::Key_Down){
             findNext();
             return true;
         }
     }

     return QWidget::eventFilter(obj, event);
}

void SearchPane::setReadOnly(bool readOnly)
{
    replaceTextBox->setEnabled(!readOnly);
    replaceOneButton->setEnabled(!readOnly);
    replaceAndFindButton->setEnabled(!readOnly);
    replaceAllButton->setEnabled(!readOnly);
}

bool SearchPane::isReadOnly() const
{
    return replaceTextBox->isEnabled();
}
