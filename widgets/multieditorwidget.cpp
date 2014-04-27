#include "multieditorwidget.h"
#include "util/iconutil.h"
#include "util/widgethelper.h"
#include "code_parser/plsql/plsqlscanner.h"
#include "code_parser/plsql/plsqlparsehelper.h"
#include "code_parser/plsql/plsqlparsingtable.h"
#include "codeeditor/blockdata.h"
#include "beans/codecollapseposition.h"
#include "beans/tokeninfo.h"
#include "util/strutil.h"
#include <QtGui>

MultiEditorWidget::MultiEditorWidget(bool enableCodeCollapsing, QWidget *parent) :
    QWidget(parent),
    currentEditor(0),
    splitDirectionGroup(0),
    infoLabel(0),
    queryScheduler(0),
    enableCodeCollapsing(enableCodeCollapsing),
    timerId(-1),
    lastEditedWordPosition(-1)
{
    createUi();
}

MultiEditorWidget::~MultiEditorWidget()
{
    //qDeleteAll(collapsePositions);
    //collapsePositions.clear();
}

void MultiEditorWidget::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    Q_ASSERT(this->queryScheduler == 0);

    this->queryScheduler = queryScheduler;

    foreach(CodeEditorAndSearchPaneWidget *editor, editors){
        editor->setQueryScheduler(queryScheduler);
    }
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

    if(this->queryScheduler != 0){
        editor->setQueryScheduler(this->queryScheduler);
    }

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
    connect(editor->editor(), SIGNAL(needsCompletionList()), this, SIGNAL(needsCompletionList()));

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

    if(!currentTextEditor()->blockChanges() && !(CodeEditor::keywordCaseFolding == CodeEditor::NoCaseFolding &&
                                                 CodeEditor::identifierCaseFolding == CodeEditor::NoCaseFolding)){
        currentTextEditor()->setBlockChanges(true);
        applyCaseFoldingRules(position, charsRemoved, charsAdded);
        currentTextEditor()->setBlockChanges(false);
    }
}

void MultiEditorWidget::applyCaseFoldingRules(int position, int charsRemoved, int charsAdded)
{
    if(charsAdded > 1000000){ //for performance
        return;
    }

    int changedCount = qAbs(charsAdded - charsRemoved);
    QTextCursor cur = currentTextEditor()->textCursor();
    int initialCursorPosition = cur.position();
    cur.setPosition(position);
    QTextBlock currentBlock = cur.block();
    if(!currentBlock.isValid()){
        return;
    }
    BlockData *blockData = static_cast<BlockData*>(currentBlock.userData());
    Q_ASSERT(blockData);

    int currentPosition = position;
    int currentBlockPosition = currentBlock.position();

    TokenInfo *token = blockData->tokenAtPosition(currentPosition - currentBlockPosition);
    if(token == 0){
        token = blockData->firstTokenFor(currentPosition - currentBlockPosition);
    }

    if(token!=0 && (isId(token) || isKeyword(token))){
        cur.setPosition(currentBlockPosition + token->startPos);
        cur.setPosition(cur.position() + (token->endPos - token->startPos), QTextCursor::KeepAnchor);
        QString selection = cur.selectedText();

        if(lastEditedWordPosition == currentBlockPosition + token->startPos){
            lastEditedWord.chop(lastEditedWord.length()+lastEditedWordPosition - position);
            if(selection.length() > lastEditedWord.length()){
                lastEditedWord.append(selection.right(selection.length() - lastEditedWord.length()));
            }
        }else{
            lastEditedWordPosition = currentBlockPosition + token->startPos;
            lastEditedWord = selection;
        }
    }else{
        lastEditedWordPosition = -1;
        lastEditedWord.clear();
    }

    cur.beginEditBlock();

    bool done=false;
    while(currentPosition <= position + changedCount){

        while(token == 0){ //no tokens left on this block, continue with next one
            currentBlock = currentBlock.next();

            if(!currentBlock.isValid()){
                done = true;
                break;
            }
            currentBlockPosition = currentBlock.position();

            blockData = static_cast<BlockData*>(currentBlock.userData());
            Q_ASSERT(blockData);

            token = blockData->firstTokenFor(0);
        }

        if(done){
            break;
        }

        if(isId(token) || isKeyword(token)){

            int absTokenPosition = currentBlockPosition + token->startPos;
            cur.setPosition(absTokenPosition);
            cur.setPosition(cur.position() + (token->endPos - token->startPos), QTextCursor::KeepAnchor);
            QString selection = cur.selectedText();
            bool needToReplace = false;

            applyCaseFolding(isId(token) ?
                                 CodeEditor::identifierCaseFolding
                                         :
                                 CodeEditor::keywordCaseFolding,
                             absTokenPosition, selection, needToReplace);

            if(needToReplace){
                cur.insertText(selection);
            }
        }

        currentPosition = currentBlockPosition + token->endPos + 1;
        token = blockData->firstTokenFor(currentPosition - currentBlockPosition);
    }

    if(cur.position() != initialCursorPosition){
        cur.setPosition(initialCursorPosition);
        currentTextEditor()->setTextCursor(cur);
    }

    cur.endEditBlock();
}

void MultiEditorWidget::applyCaseFolding(CodeEditor::CaseFoldingType foldingType, int textPosition, QString &text, bool &changed)
{
    changed = false;

    //CodeEditor::CaseFoldingType foldingType = keyword ? CodeEditor::keywordCaseFolding : CodeEditor::identifierCaseFolding;
    if(foldingType == CodeEditor::NoCaseFolding &&
            lastEditedWordPosition == textPosition &&
            lastEditedWord.compare(text, Qt::CaseSensitive)!=0){
        text = lastEditedWord;
        changed = true;
    }else if(foldingType == CodeEditor::UpperCaseFolding && !isUpperCase(text)){
        text = text.toUpper();
        changed = true;
    }else if(foldingType == CodeEditor::LowerCaseFolding && !isLowerCase(text)){
        text = text.toLower();
        changed = true;
    }
}

bool MultiEditorWidget::isId(TokenInfo *token) const
{
    return token->tokenOrRuleId==PLS_ID ||
            (token->tokenOrRuleId < NON_LITERAL_START_IX && (token->endPos - token->startPos) < MIN_KEYWORD_LENGTH);
}

bool MultiEditorWidget::isKeyword(TokenInfo *token) const
{
    return token->tokenOrRuleId < NON_LITERAL_START_IX && (token->endPos - token->startPos) >= MIN_KEYWORD_LENGTH;
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
