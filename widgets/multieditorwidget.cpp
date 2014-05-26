#include "multieditorwidget.h"
#include "util/iconutil.h"
#include "util/widgethelper.h"
#include "code_parser/plsql/plsqlscanner.h"
#include "code_parser/plsql/plsqlparsehelper.h"
#include "code_parser/plsql/plsqlparsingtable.h"
#include "code_parser/plsql/plsqltreebuilder.h"
#include "codeeditor/blockdata.h"
#include "beans/codecollapseposition.h"
#include "beans/tokeninfo.h"
#include "util/strutil.h"
#include "connection_page/dbuimanager.h"
#include "code_structure_tree/codestructurepane.h"
#include <QtGui>

MultiEditorWidget::MultiEditorWidget(DbUiManager *uiManager, bool plsqlMode, QWidget *parent) :
    QWidget(parent),
    uiManager(uiManager),
    currentEditor(0),
    splitDirectionGroup(0),
    infoLabel(0),
    queryScheduler(0),
    plsqlMode(plsqlMode),
    lastEditedWordPosition(-1),
    treeBuilder(0),
    lastChangeTime(QTime::currentTime()),
    lastParseLengthInMs(1000000),
    lastMarkedCursorPos(-1)
{
    createUi();
}

MultiEditorWidget::~MultiEditorWidget()
{
    //qDeleteAll(collapsePositions);
    //collapsePositions.clear();

    uiManager->getCodeStructurePane()->unregisterWidget(this);
    delete this->treeBuilder;
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
    connect(&reparseTimer, SIGNAL(timeout()), this, SLOT(onReparseTimer()));

    connect(&codeReparser, SIGNAL(parsingCompleted(int,bool,PlSqlTreeBuilder*,int)), this, SLOT(parsingCompleted(int,bool,PlSqlTreeBuilder*,int)));
    reparseTimer.start(300);
}

CodeEditorAndSearchPaneWidget *MultiEditorWidget::getCurrentEditor() const
{
    Q_ASSERT(currentEditor);
    return currentEditor;
}

QList<QAction*> MultiEditorWidget::addSplittingActions(QWidget *w)
{
    Q_ASSERT(splitDirectionGroup==0);

    int currentActionCount = w->actions().size();

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
    w->addAction(separator);

    splitDirectionGroup = WidgetHelper::addSplitDirectionActions(w);

    splitDirectionGroup->setEnabled(false);
    connect(splitDirectionGroup, SIGNAL(triggered(QAction*)), this, SLOT(editorOrientationActionSelected(QAction*)));

    QList<QAction*> addedActions;
    QList<QAction*> allActions = w->actions();
    for(int i=currentActionCount; i<allActions.size(); ++i){
        addedActions.append(allActions.at(i));
    }

    return addedActions;
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

void MultiEditorWidget::setInitialText(const QString &text)
{
    getCurrentEditor()->setInitialText(text);

    reparse();
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

    if(focusIn && plsqlMode){
        uiManager->getCodeStructurePane()->setCurrentWidget(this);
        uiManager->getCodeStructurePane()->setTreeBuilder(this, this->treeBuilder, currentEditor->editor()->textCursor().position());
    }
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
    CodeEditorAndSearchPaneWidget *editor=new CodeEditorAndSearchPaneWidget(this->plsqlMode);

    if(this->queryScheduler != 0){
        editor->setQueryScheduler(this->queryScheduler);
    }

    connect(editor, SIGNAL(escapeKeyPressed()), this, SIGNAL(escapeKeyPressed()));
    editors.append(editor);

    if(editors.size()>1){
        editor->editor()->setDocument(editors.at(0)->editor()->document()); //bind all instances to the same document (first created one)
        editor->editor()->setLastParseId(editors.at(0)->editor()->getLastParseId());
    }

    if(editors.size()==1){
        currentEditor=editor;
    }

    connect(editor, SIGNAL(focusEvent(QWidget*,bool)), this, SLOT(codeEditorFocusEvent(QWidget*,bool)));
    connect(editor->editor(), SIGNAL(cursorPositionChanged()), this, SLOT(cursorPositionChanged()));
    connect(editor->editor(), SIGNAL(updated(CodeEditor*)), this, SLOT(updateEditors(CodeEditor*)));
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

    lastPosChangeTime = QTime::currentTime();
}

void MultiEditorWidget::documentContentsChanged(int position, int charsRemoved, int charsAdded)
{
    if(!currentTextEditor()->blockChanges() && !(CodeEditor::keywordCaseFolding == CodeEditor::NoCaseFolding &&
                                                 CodeEditor::identifierCaseFolding == CodeEditor::NoCaseFolding)){
        currentTextEditor()->setBlockChanges(true);
        applyCaseFoldingRules(position, charsRemoved, charsAdded);
        currentTextEditor()->setBlockChanges(false);

    }

    lastChangeTime = QTime::currentTime();
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

void MultiEditorWidget::onReparseTimer()
{
    int reparseInterval;

    if(lastParseLengthInMs <= 200){
        reparseInterval = 300;
    }else if(lastParseLengthInMs <= 500){
        reparseInterval = 1000;
    }else if(lastParseLengthInMs <= 1000){
        reparseInterval = 1500;
    }else{
        reparseInterval = 3000;
    }

    QTime now = QTime::currentTime();
    if(lastParseTime < lastChangeTime && lastChangeTime.msecsTo(now) >= reparseInterval){
        reparse();
    }

    int cursorPos = currentTextEditor()->textCursor().position();
    if(plsqlMode && lastMarkedCursorPos!=cursorPos && lastPosChangeTime.msecsTo(now) >= 300){
        lastMarkedCursorPos = cursorPos;
        uiManager->getCodeStructurePane()->setCursorPosition(this, cursorPos);
    }

    foreach(CodeEditorAndSearchPaneWidget *editor, editors){
        editor->editor()->timerTick();
    }
}

void MultiEditorWidget::reparse()
{
    lastParseTime = QTime::currentTime();

    codeReparser.parse(currentTextEditor()->toPlainText());
}

void MultiEditorWidget::parsingCompleted(int requestId, bool success, PlSqlTreeBuilder *treeBulder, int elapsedTime)
{
    Q_UNUSED(success);
    Q_UNUSED(elapsedTime);

    lastParseLengthInMs = elapsedTime;

    if(lastChangeTime > lastParseTime){
        delete treeBulder;
        return;
    }

    QList<CodeCollapsePosition*> collapsePositions = treeBulder->getCollapsePositions();
    if(collapsePositions.size()>0){
        QTextDocument *doc = currentTextEditor()->document();
        for(int i=0; i<collapsePositions.size(); ++i){
            CodeCollapsePosition *pos = collapsePositions.at(i);
            QTextBlock block = doc->findBlockByNumber(pos->startLine);
            if(!block.isValid()){
                continue;
            }

            BlockData *data = static_cast<BlockData*>(block.userData());
            Q_ASSERT(data);
            data->setCollapsePosition(pos->endLine, requestId);
        }

        treeBulder->clearCollapsePositions();
    }

    foreach(CodeEditorAndSearchPaneWidget *editor, editors){
        editor->editor()->setLastParseId(requestId);
    }

    if(plsqlMode){
        PlSqlTreeBuilder *currentTreeBuilder = this->treeBuilder;

        this->treeBuilder = treeBulder;
        uiManager->getCodeStructurePane()->setTreeBuilder(this, treeBuilder, currentTextEditor()->textCursor().position());

        delete currentTreeBuilder;
    }else{
        delete treeBulder;
    }
}

void MultiEditorWidget::updateEditors(CodeEditor *except)
{
    foreach(CodeEditorAndSearchPaneWidget *editor, editors){
        if(editor->editor()!=except){
            editor->editor()->updateAllParts();
        }
    }
}
