#include "codeeditor.h"
#include "syntaxhighligher.h"
#include "linenumberarea.h"
#include "linenavigationbar.h"
#include "dialogs/gotolinedialog.h"
#include "util/widgethelper.h"
#include "app_menu/appmenu.h"
#include "app_menu/appeditmenu.h"
#include <QPainter>

#define EDITOR_SEARCH_BG_COLOR QColor(Qt::yellow)
#define EDITOR_ERROR_TEXT_COLOR Qt::red
#define EDITOR_CURR_LINE_NAVBAR_COLOR palette().color(QPalette::Window).darker(130)

QFont CodeEditor::currentFont;

CodeEditor::CodeEditor(QWidget *parent) :
    QPlainTextEdit(parent), lineNumberArea(0), lineNavBar(0)
{
    QFont monospaceFont("Monospace");
    setFont(monospaceFont);

    lineNumberArea = new LineNumberArea(this);
    lineNavBar = new LineNavigationBar(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    //connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateNavBarHighlightColors()));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();

    setTabStopWidth(30);
    setWordWrapMode(QTextOption::NoWrap);
    new SyntaxHighligher(this->document());

    QPalette p=palette();
    p.setColor(QPalette::Disabled, QPalette::Base, p.color(QPalette::Window));
    setPalette(p);

    setFont(CodeEditor::currentFont);

    strTab=QString("   ");

    connect(this, SIGNAL(undoAvailable(bool)), this, SLOT(setUndoAvailable(bool)));
    connect(this, SIGNAL(redoAvailable(bool)), this, SLOT(setRedoAvailable(bool)));
    connect(this, SIGNAL(textChanged()), this, SLOT(removeErrorSelection()));

    new QShortcut(QKeySequence(tr("Ctrl+=", "CodeEditor|Increase font size")),
                  this, SLOT(increaseFontSize()), 0, Qt::WidgetWithChildrenShortcut);
}

//use only when sure that there's not lots of text in editor
QStringList CodeEditor::getSemicolonSeparated() const
{
    return toPlainText().split(';', QString::SkipEmptyParts);
}

int CodeEditor::lineNumberAreaWidth()
 {
     int digits = 1;
     int max = qMax(1, blockCount());
     while (max >= 10) {
         max /= 10;
         ++digits;
     }

     int space = 5 + 5 + fontMetrics().width(QLatin1Char('9')) * digits;

     return space;
}

 void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
 {
     bool scrollbarVisible = verticalScrollBar()->isVisibleTo(this);
     lineNavBar->setVisible(scrollbarVisible);
     int rightMargin = scrollbarVisible ? lineNavBar->width() : 0;

     setViewportMargins(lineNumberAreaWidth(), 0, rightMargin, 0);
 }



 void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
 {
     if (dy)
         lineNumberArea->scroll(0, dy);
     else
         lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

     if (rect.contains(viewport()->rect()))
         updateLineNumberAreaWidth(0);
 }

 void CodeEditor::setUndoAvailable(bool available)
 {
     if(qApp->focusWidget()==this){
        AppMenu::defaultInstance()->getEditMenu()->setUndoEnabled(available);
     }
 }

 void CodeEditor::setRedoAvailable(bool available)
 {
     if(qApp->focusWidget()==this){
         AppMenu::defaultInstance()->getEditMenu()->setRedoEnabled(available);
     }
 }

 void CodeEditor::removePulsatePositions()
 {
     pulsatePositions.clear();
     highlightCurrentLine();
 }

 void CodeEditor::removeErrorSelection()
 {
     if(!errorPosition.isNull()){
         errorPosition=QTextCursor();
         highlightCurrentLine();
         updateNavBar();
     }
 }

 void CodeEditor::resizeEvent(QResizeEvent *e)
 {
     QPlainTextEdit::resizeEvent(e);

     QRect cr = contentsRect();
     lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
     lineNavBar->setGeometry(QRect(cr.right()-lineNavBar->width()+1, cr.top(), lineNavBar->width(), cr.height()));

     if(lineNavBar->isVisible()!=verticalScrollBar()->isVisible()){
         updateLineNumberAreaWidth(0);
     }
 }



 void CodeEditor::highlightCurrentLine()
 {
     QList<QTextEdit::ExtraSelection> extraSelections;

     if (!isReadOnly()) {
         QTextEdit::ExtraSelection selection;

         QColor lineColor = QColor(Qt::yellow).lighter(180);

         selection.format.setBackground(lineColor);
         selection.format.setProperty(QTextFormat::FullWidthSelection, true);
         selection.cursor = textCursor();
         selection.cursor.clearSelection();
         extraSelections.append(selection);
     }

     for(int i=0; i<foundTextPositions.size(); ++i){
         QTextEdit::ExtraSelection searchTextSelection;
         searchTextSelection.format.setBackground(EDITOR_SEARCH_BG_COLOR);
         searchTextSelection.format.setForeground(QColor(Qt::black));
         searchTextSelection.cursor=foundTextPositions.at(i);

         extraSelections.append(searchTextSelection);
     }

     for(int i=0; i<pulsatePositions.size(); ++i){
         QTextEdit::ExtraSelection pulsateSelection;
         pulsateSelection.format.setBackground(QColor(210,255,165));
         pulsateSelection.cursor=pulsatePositions.at(i);

         extraSelections.append(pulsateSelection);
     }

     if(!errorPosition.isNull()){
         QTextEdit::ExtraSelection errorSelection;
         errorSelection.format.setUnderlineStyle(QTextCharFormat::WaveUnderline);
         errorSelection.format.setUnderlineColor(Qt::red);
         //errorSelection.format.setBackground(Qt::red);
         errorSelection.format.setForeground(EDITOR_ERROR_TEXT_COLOR);
         errorSelection.cursor=errorPosition;

         extraSelections.append(errorSelection);
     }

     setExtraSelections(extraSelections);
 }



 void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
 {
     QPainter painter(lineNumberArea);
     QRect rect=event->rect();
     painter.fillRect(rect, palette().color(QPalette::Base));
     painter.setPen(Qt::DotLine);
     painter.drawLine(rect.right(), rect.top(), rect.right(), rect.bottom());

     QTextBlock block = firstVisibleBlock();
     int blockNumber = block.blockNumber();
     int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
     int bottom = top + (int) blockBoundingRect(block).height();

     while (block.isValid() && top <= event->rect().bottom()) {
         if (block.isVisible() && bottom >= event->rect().top()) {
             QString number = QString::number(blockNumber + 1);
             painter.drawText(0, top, lineNumberArea->width()-5, fontMetrics().height(),
                              Qt::AlignRight, number);
         }

         block = block.next();
         top = bottom;
         bottom = top + (int) blockBoundingRect(block).height();
         ++blockNumber;
     }
 }


 void CodeEditor::lineNavBarPaintEvent(QPaintEvent *event)
 {
     QPainter painter(lineNavBar);
     QRect rect = event->rect();
     painter.fillRect(rect, palette().color(QPalette::Window));
     painter.setPen(Qt::DotLine);
     painter.drawLine(rect.left(), rect.top(), rect.left(), rect.bottom());

     if(blockCount()<1){
         return;
     }

     drawLineNavBarHighlights(painter);
 }

 void CodeEditor::drawLineNavBarHighlights(QPainter &painter)
 {
     qreal blockHeight = blockBoundingRect(document()->findBlockByNumber(0)).height();
     qreal docHeight = blockCount() * blockHeight;
     bool hasScrollbars = docHeight > viewport()->height();

     for(int i=0; i<foundTextPositions.size(); ++i){
         const QTextCursor &cur=foundTextPositions.at(i);
         Q_ASSERT(!cur.isNull());
         drawLineNavBarHighlight(painter, cur.blockNumber(), EDITOR_SEARCH_BG_COLOR, docHeight, blockHeight, hasScrollbars);
     }

     if(!errorPosition.isNull()){
         drawLineNavBarHighlight(painter, errorPosition.blockNumber(), EDITOR_ERROR_TEXT_COLOR, docHeight, blockHeight, hasScrollbars);
     }

     //highlight current line
     drawLineNavBarHighlight(painter,
                             textCursor().blockNumber(),
                             EDITOR_CURR_LINE_NAVBAR_COLOR,
                             docHeight, blockHeight, hasScrollbars);
 }

 void CodeEditor::drawLineNavBarHighlight(QPainter &painter,
                                          int blockNumber,
                                          const QColor &color,
                                          qreal docHeight,
                                          qreal blockHeight,
                                          bool hasScrollbars)
 {
     qreal highlightTop;
     qreal highlightHeight;
     if(!hasScrollbars){ //height of text is less than height of editor (scrollbar not visible)
         QRectF blockRect = this->blockBoundingGeometry(document()->findBlockByNumber(blockNumber)).translated(contentOffset());
         highlightTop = blockRect.top();
         highlightHeight = blockRect.height();
     }else{
         qreal ratio = viewport()->height() / docHeight;
         highlightTop = blockNumber * blockHeight * ratio;
         highlightHeight = blockHeight * ratio;
         if(highlightHeight<3){
             highlightHeight=3;
         }

         if(blockNumber==blockCount()-1){
             highlightTop -= 3;
         }
     }

     painter.fillRect(QRect(1,(int)highlightTop,lineNavBar->width()-1,(int)highlightHeight), color);
 }

 void CodeEditor::lineNavBarMouseReleaseEvent(QMouseEvent *event)
 {
     if(blockCount()==0){
         return;
     }

     qreal clickYPos=event->posF().y();

     qreal blockHeight = blockBoundingRect(document()->findBlockByNumber(0)).height();
     qreal docHeight = blockCount() * blockHeight;
     bool hasScrollbars = docHeight > viewport()->height();

     int blockToScrollTo=0;
     if(!hasScrollbars){
         blockToScrollTo = (int) clickYPos / blockHeight;
     }else{
         qreal ratio = viewport()->height() / docHeight;
         blockToScrollTo = (int) clickYPos / (blockHeight * ratio);
         //qDebug() << "blockToScrollTo =" << blockToScrollTo;

         int closestBlockNo=-1;
         if(ratio<0.25){ //user might have difficulties clicking on right position, so try to adjust to closest highlighted block
             QList<int> keys = getHighlightedBlockNumbers();
             for(int i=0; i<keys.size(); ++i){
                 int blockNo = keys.at(i);
                 qreal blockHighlightPos = blockNo * blockHeight * ratio;
                 int diff = qAbs(clickYPos-blockHighlightPos);
                 if(diff<=10 && (qAbs(blockNo-blockToScrollTo)<qAbs(closestBlockNo-blockToScrollTo)
                                    || closestBlockNo==-1)){
                     closestBlockNo=blockNo;
                 }
             }
         }

         if(closestBlockNo!=-1){
             blockToScrollTo=closestBlockNo;
             //qDebug() << "asjusted to" << blockToScrollTo;
         }
     }

     if(blockToScrollTo > blockCount()-1){
         blockToScrollTo = blockCount() - 1 ;
     }

     QTextCursor cur = textCursor();
     cur.setPosition(document()->findBlockByNumber(blockToScrollTo).position());
     setTextCursor(cur);
     centerCursor();
 }

 QList<int> CodeEditor::getHighlightedBlockNumbers() const
 {
     QList<int> results;
     results.reserve(foundTextPositions.size()+1);

     for(int i=0; i<foundTextPositions.size(); ++i){
         const QTextCursor &cur=foundTextPositions.at(i);
         results.append(cur.blockNumber());
     }

     if(!errorPosition.isNull()){
         results.append(errorPosition.blockNumber());
     }

     return results;
 }

 void CodeEditor::lineNumberAreaWheelEvent(QWheelEvent *event)
 {
     this->wheelEvent(event);
 }

 void CodeEditor::lineNavBarWheelEvent(QWheelEvent *event)
 {
     this->wheelEvent(event);
 }

 void CodeEditor::keyReleaseEvent ( QKeyEvent * event )
 {
     QPlainTextEdit::keyReleaseEvent(event);

     if(event->key()==Qt::Key_Escape){
        emit escapeKeyPressed();
     }
 }

 void CodeEditor::keyPressEvent ( QKeyEvent * event )
 {
     bool handled=false;

     if(!isReadOnly()){
         int key=event->key();
         if((key==Qt::Key_Return || key==Qt::Key_Enter) && event->modifiers()==0){
            handled=true;
            textCursor().beginEditBlock();
            QPlainTextEdit::keyPressEvent(event);
            autoIndentNewBlock();
            textCursor().endEditBlock();
         }else if(key==Qt::Key_Tab){
             handled=true;

             if(textCursor().hasSelection()){
                 if(event->modifiers() & Qt::ShiftModifier){
                     unindentSelection();
                 }else{
                     indentSelection();
                 }
             }else{
                 textCursor().insertText(strTab);
             }
         }else if(key==Qt::Key_Backtab && textCursor().hasSelection()){
             handled=true;
             unindentSelection();
         }else if(key==Qt::Key_Home && (Qt::ControlModifier & event->modifiers())!=Qt::ControlModifier){
             handled=true;
             handleHomeKey(Qt::ShiftModifier & event->modifiers());
         }else if(key==Qt::Key_Up && (event->modifiers()==(Qt::ControlModifier+Qt::ShiftModifier))){
             handled=true;
             moveSelectionUp();
         }else if(key==Qt::Key_Down && (event->modifiers()==(Qt::ControlModifier+Qt::ShiftModifier))){
             handled=true;
             moveSelectionDown();
         }else if(event->matches(QKeySequence::Cut)){
             handled=true;
             customCut();
         }else if(event->matches(QKeySequence::Copy)){
             handled=true;
             customCopy();
         }
     }

     if(!handled){
        QPlainTextEdit::keyPressEvent(event);
     }
 }

 void CodeEditor::focusInEvent(QFocusEvent *event)
 {
     QPlainTextEdit::focusInEvent(event);

     emit gotFocus();
 }

 void CodeEditor::focusOutEvent(QFocusEvent *event)
 {
     QPlainTextEdit::focusOutEvent(event);

     emit lostFocus();
 }

 void CodeEditor::contextMenuEvent(QContextMenuEvent *event)
 {
     QMenu *menu = new QMenu();

     AppEditMenu *editMenu=AppMenu::defaultInstance()->getEditMenu();
     menu->addAction(editMenu->editUndoAction);
     menu->addAction(editMenu->editRedoAction);
     menu->addSeparator();
     menu->addAction(editMenu->editCutAction);
     menu->addAction(editMenu->editCopyAction);
     menu->addAction(editMenu->editCopyAsAction);
     menu->addAction(editMenu->editPasteAction);
     menu->addSeparator();
     menu->addAction(editMenu->editCommentAction);
     menu->addAction(editMenu->editMoveUpAction);
     menu->addAction(editMenu->editMoveDownAction);
     menu->addAction(editMenu->editSelectBlockAction);
     menu->addAction(editMenu->editToUpperCaseAction);
     menu->addAction(editMenu->editToLowerCaseAction);
     menu->addAction(editMenu->editCreateDuplicateAction);

     menu->exec(event->globalPos());
     delete menu;
 }

 bool CodeEditor::event(QEvent *e)
 {
     if(e->type()==QEvent::FontChange && lineNumberArea){
         QFont newFont=this->font();

         lineNumberArea->setFont(newFont);

         CodeEditor::currentFont = newFont;
     }

     return QPlainTextEdit::event(e);
 }


 void CodeEditor::autoIndentNewBlock()
 {
     QTextCursor cur=textCursor();
     if(!cur.movePosition(QTextCursor::PreviousBlock)){
         return;
     }
     QString prevLine=cur.block().text();
     QString prefix;
     QChar c;
     for(int i=0; i<prevLine.size(); ++i){
         c=prevLine.at(i);
         if(!c.isSpace()){
             break;
         }
         prefix.append(c);
     }
     cur.movePosition(QTextCursor::NextBlock);
     if(!prefix.isEmpty()){
        cur.insertText(prefix);
        setTextCursor(cur);
     }

 }

 void CodeEditor::indentSelection()
 {
     QTextCursor cur=textCursor();
     if(!cur.hasSelection()){
         return;
     }
     cur.beginEditBlock();
     CursorPositionInfo inf=getStartStopPositions(cur);

     cur.setPosition(inf.startPos);
     cur.movePosition(QTextCursor::StartOfBlock);
     int blocksToMove=(inf.endBlock-inf.startBlock)+1;
     for(int i=0; i<blocksToMove; ++i){
        cur.insertText(strTab);
        cur.movePosition(QTextCursor::NextBlock);
     }
     cur.endEditBlock();

     inf.startPos+=strTab.size();
     inf.endPos+=blocksToMove*strTab.size();
     inf.selectText(cur);

     setTextCursor(cur);
 }

 void CodeEditor::unindentSelection()
 {
     QTextCursor cur=textCursor();
     if(!cur.hasSelection()){
         return;
     }
     cur.beginEditBlock();
     CursorPositionInfo inf=getStartStopPositions(cur);

     cur.setPosition(inf.startPos);
     //cur.movePosition(QTextCursor::StartOfBlock);
     int blocksToMove=(inf.endBlock-inf.startBlock)+1;
     for(int i=0; i<blocksToMove; ++i){
        moveToFirstNonSpaceCharacter(cur);
        for(int k=0; k<strTab.size(); ++k){
            if(cur.positionInBlock()>0){
                cur.deletePreviousChar();
                if(i==0){
                    --inf.startPos;
                }

                --inf.endPos;
            }else{
                break;
            }
        }

        cur.movePosition(QTextCursor::NextBlock);
     }
     cur.endEditBlock();
     inf.selectText(cur);
     setTextCursor(cur);
 }

 void CodeEditor::handleHomeKey(bool keepSelection)
 {
     QTextCursor cur=textCursor();
     bool onlySpaces;
     int ix=getFirstNonSpaceCharacterIndex(cur, &onlySpaces);
     if(!cur.atBlockStart() && cur.positionInBlock()<=ix){
         cur.movePosition(QTextCursor::StartOfBlock, keepSelection ? QTextCursor::KeepAnchor : QTextCursor::MoveAnchor);
     }else{
         moveToFirstNonSpaceCharacter(cur, keepSelection ? QTextCursor::KeepAnchor : QTextCursor::MoveAnchor);
     }
     setTextCursor(cur);
 }

 int CodeEditor::getFirstNonSpaceCharacterIndex(QTextCursor &cur, bool *onlySpaces)
 {
     QString line=cur.block().text();
     QChar c;
     int firstNonSpaceIx=-1;
     for(int i=0; i<line.size(); ++i){
         c=line.at(i);
         if(!c.isSpace()){
             firstNonSpaceIx=i;
             break;
         }
     }

     if(onlySpaces!=0){
        *onlySpaces=(firstNonSpaceIx==-1 && line.size()>0);
         if(*onlySpaces){
             firstNonSpaceIx=line.size();
         }
     }
     return firstNonSpaceIx;
 }

 bool CodeEditor::moveToFirstNonSpaceCharacter(QTextCursor &cur, QTextCursor::MoveMode moveMode)
 {
    bool onlySpaces;
    int ix = getFirstNonSpaceCharacterIndex(cur, &onlySpaces);
    cur.setPosition(cur.block().position()+ix, moveMode);
    return !onlySpaces;
 }

 void CodeEditor::removeFromBeginning(QTextCursor &cur, int characterCount){
     moveToFirstNonSpaceCharacter(cur);
     for(int k=0; k<characterCount; ++k){
         cur.deleteChar();
     }
 }

 void CodeEditor::changeCase(bool toUpper)
 {
     QTextCursor cur=textCursor();
     int startPos=cur.anchor();
     if(cur.hasSelection()){
        int endPos=cur.position();
        cur.insertText(toUpper ? cur.selectedText().toUpper() : cur.selectedText().toLower());
        cur.setPosition(startPos);
        cur.setPosition(endPos, QTextCursor::KeepAnchor);
     }else{
         cur.select(QTextCursor::WordUnderCursor);
         if(cur.hasSelection()){
             cur.insertText(toUpper ? cur.selectedText().toUpper() : cur.selectedText().toLower());
         }
         cur.setPosition(startPos);
     }
     setTextCursor(cur);
 }

 CursorPositionInfo CodeEditor::getStartStopPositions(const QTextCursor cur)
 {
     CursorPositionInfo info;

     int startPos=cur.anchor();
     int endPos=cur.position();
     if(startPos>endPos){
         qSwap(startPos, endPos);
         info.anchorAtEnd=true;
     }else{
         info.anchorAtEnd=false;
     }
     int startBlock=cur.document()->findBlock(startPos).blockNumber();
     int endBlock=cur.document()->findBlock(endPos).blockNumber();

     info.startPos=startPos;
     info.endPos=endPos;
     info.startBlock=startBlock;
     info.endBlock=endBlock;

     return info;
 }

 void CodeEditor::moveSelectionUp()
 {
     QTextCursor cur=textCursor();
     if(cur.movePosition(QTextCursor::PreviousBlock, QTextCursor::KeepAnchor)){
         setTextCursor(cur);
     }
 }

 void CodeEditor::moveSelectionDown()
 {
     QTextCursor cur=textCursor();
     if(cur.movePosition(QTextCursor::NextBlock, QTextCursor::KeepAnchor)){
         cur.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
         setTextCursor(cur);
     }
 }

 void CodeEditor::ensureHasSelection()
 {
     QTextCursor cur=textCursor();
     if(!cur.hasSelection()){
         cur.select(QTextCursor::WordUnderCursor);
         setTextCursor(cur);
     }
 }


 void CodeEditor::updateNavBar()
 {
     lineNavBar->update();
 }

 void CodeEditor::commentBlocks()
 {
     QString comment="--";
     QTextCursor cur=textCursor();
     CursorPositionInfo inf=getStartStopPositions(cur);

     cur.setPosition(inf.startPos);
     int blocksToMove=(inf.endBlock-inf.startBlock)+1;
     int addComments=-1;

     cur.beginEditBlock();

     /*
    int minPosToStart=-1;
    for(int i=0; i<blocksToMove; ++i){
        bool onlySpaces;
        int blockMin=getFirstNonSpaceCharacterIndex(cur, &onlySpaces);
        if(minPosToStart==-1){
            minPosToStart=blockMin;
        }
        if(!onlySpaces && blockMin<minPosToStart){
            minPosToStart=blockMin;
        }
        cur.movePosition(QTextCursor::NextBlock);
    }*/

     cur.setPosition(inf.startPos);
     for(int i=0; i<blocksToMove; ++i){
         QString blockText=cur.block().text().trimmed();
         if(!blockText.isEmpty()){
             if(addComments==-1){
                 addComments=blockText.startsWith(comment) ? 0 : 1;
             }

             if(addComments==1){
                 cur.setPosition(cur.block().position());
                 cur.insertText(comment);
                 inf.endPos+=comment.size();
             }else if(addComments==0 && blockText.startsWith(comment)){
                 removeFromBeginning(cur, comment.size());
                 inf.endPos-=comment.size();
             }
         }
         cur.movePosition(QTextCursor::NextBlock);
     }
     if(addComments==1){
         inf.startPos+=comment.size();
     }else if(addComments==0){
         inf.startPos-=comment.size();
     }
     cur.endEditBlock();
     inf.selectText(cur);
     setTextCursor(cur);
 }

 void CodeEditor::goToLine()
 {
     QTextCursor cur=textCursor();
     GoToLineDialog dialog(cur.blockNumber()+1, document()->blockCount(), this);
     dialog.setWindowModality(Qt::WindowModal);
     if(dialog.exec()){
         int line = dialog.getLine()-1;
         cur.setPosition(document()->findBlockByNumber(line).position());
         setTextCursor(cur);
         centerCursor();
     }
 }

 void CodeEditor::increaseFontSize()
 {
    WidgetHelper::changeFontSize(this, 0.5);
 }

 void CodeEditor::decreaseFontSize()
 {
     WidgetHelper::changeFontSize(this, -0.5);
 }

 void CodeEditor::resetFontSize()
 {
     this->setFont(WidgetHelper::getMonospaceFont(qApp->font(this).pointSize()));
 }

 void CodeEditor::toUpperCase()
 {
     changeCase(true);
 }

 void CodeEditor::toLowerCase()
 {
     changeCase(false);
 }

 void CodeEditor::makeDuplicate()
 {
    QTextCursor cur=textCursor();
    CursorPositionInfo inf=getStartStopPositions(cur);
    if(!cur.hasSelection()){
        cur.movePosition(QTextCursor::StartOfBlock);
        cur.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
    }
    QString blockText=cur.selectedText();
    cur.setPosition(document()->findBlockByNumber(inf.endBlock).position());
    if(inf.endBlock==document()->blockCount()-1){
        cur.movePosition(QTextCursor::EndOfBlock);
        cur.insertBlock();
    }else{
        blockText.append("\n");
    }
    cur.movePosition(QTextCursor::NextBlock);
    cur.insertText(blockText);

    inf.selectText(cur);
    setTextCursor(cur);
 }

 void CodeEditor::moveUp()
 {
    moveSelectedText(true);
 }

 void CodeEditor::moveDown()
 {
     moveSelectedText(false);
 }

 void CodeEditor::moveSelectedText(bool up)
 {
     QTextCursor cur=textCursor();
     CursorPositionInfo inf=getStartStopPositions(cur);

     if((up && inf.startBlock==0) || (!up && inf.endBlock>document()->blockCount()-2)){
        return;
     }

     cur.beginEditBlock();

     if(up){
        int startBlockPos=document()->findBlockByNumber(inf.startBlock).position();
        cur.setPosition(startBlockPos);
        cur.movePosition(QTextCursor::PreviousBlock);
        cur.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
        QString blockText=cur.selectedText();
        cur.movePosition(QTextCursor::StartOfBlock);
        cur.movePosition(QTextCursor::NextBlock, QTextCursor::KeepAnchor);
        cur.removeSelectedText();
        --inf.startBlock;
        --inf.endBlock;
        int endBlockPos=document()->findBlockByNumber(inf.endBlock).position();
        cur.setPosition(endBlockPos); //now previous block is deleted and cursor is set to start of last moved block
        if(inf.endBlock==document()->blockCount()-1){ //if we just deleted last line, we need to create a new empty line
            cur.movePosition(QTextCursor::End);
            cur.insertBlock();
            --inf.startPos;
            --inf.endPos;
        }else{
            blockText.append("\n");
        }
        cur.movePosition(QTextCursor::NextBlock);
        cur.insertText(blockText);
        inf.startPos-=blockText.size();
        inf.endPos-=blockText.size();
     }else{
         int endBlockPos=document()->findBlockByNumber(inf.endBlock).position();
         cur.setPosition(endBlockPos);
         cur.movePosition(QTextCursor::NextBlock);
         cur.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
         QString blockText=cur.selectedText();
         if(inf.endBlock==document()->blockCount()-2){
            cur.movePosition(QTextCursor::PreviousBlock);
            cur.movePosition(QTextCursor::EndOfBlock);
            cur.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
         }else{
            cur.movePosition(QTextCursor::NextBlock, QTextCursor::KeepAnchor);
         }
         cur.removeSelectedText();
         int startBlockPos=document()->findBlockByNumber(inf.startBlock).position();
         cur.setPosition(startBlockPos); //now next block is deleted and cursor is set to start of first selected block
         blockText.append("\n");
         cur.insertText(blockText);
         ++inf.startBlock;
         ++inf.endBlock;
         inf.startPos+=blockText.size();
         inf.endPos+=blockText.size();
     }

     inf.selectText(cur);
     setTextCursor(cur);
     cur.endEditBlock();
 }

 void CodeEditor::customCut()
 {
     if(isReadOnly()){
         return;
     }
     ensureHasSelection();
     cut();
 }

 void CodeEditor::customCopy()
 {
     ensureHasSelection();
     copy();
 }

 void CodeEditor::addText(const QString &text)
 {
     QTextCursor cur=this->textCursor();
     if(!cur.atEnd()){
         cur.movePosition(QTextCursor::End);
     }
     cur.insertText(text);
 }

 QString CodeEditor::getCurrentText(QTextCursor &txtCursor) const
 {
     QString result;

     QTextCursor cursor=textCursor();
     if(cursor.hasSelection()){
         result = cursor.selectedText().replace(QChar(0x2029), QChar('\n'));
     }else{
         result = getCurrentTextSurroundedByEmptyLines(cursor);
     }

     txtCursor=cursor;
     return result;
 }

 QString CodeEditor::getCurrentTextSurroundedByEmptyLines(QTextCursor &cursor) const
 {
     QString result;

     QString line=cursor.block().text();

     if(line.trimmed().isEmpty()){
         return result;
     }

     //move up until we find an empty line
     while(cursor.movePosition(QTextCursor::PreviousBlock)){
         line = cursor.block().text();

         if(line.trimmed().isEmpty()){
             break;
         }
     }

     if(!line.isEmpty()){
         result.append(line);
         cursor.movePosition(QTextCursor::StartOfBlock);
         if(!cursor.movePosition(QTextCursor::NextBlock, QTextCursor::KeepAnchor)){ //there is a single line in editor
             cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
             return result;
         }
     }else{
         cursor.movePosition(QTextCursor::NextBlock);
     }

     do{
         line = cursor.block().text();

         if(line.trimmed().isEmpty()){
             cursor.movePosition(QTextCursor::PreviousBlock, QTextCursor::KeepAnchor);
             cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
             break;
         }

         if(!result.isEmpty()){
             result.append("\n");
         }

         result.append(line);
     }while(cursor.movePosition(QTextCursor::NextBlock, QTextCursor::KeepAnchor));

     if(!result.isEmpty()){
         cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
     }

     return result;
 }

 void CodeEditor::setFoundTextPositions(const QList< QTextCursor > &foundTextPositions)
 {
     if(foundTextPositions.size()>0){
        this->foundTextPositions=foundTextPositions;
        highlightCurrentLine();
     }
     updateNavBar();
 }

 void CodeEditor::clearFoundTextPositions()
 {
     if(this->foundTextPositions.size()>0){
        this->foundTextPositions.clear();
        highlightCurrentLine();
        updateNavBar();
     }
 }

 void CodeEditor::showLinePosition(int line, int linePos)
 {
     QTextBlock block=this->document()->findBlockByNumber(line);
     QTextCursor cursor=this->textCursor();
     cursor.setPosition(block.position()+linePos);
     setTextCursor(cursor);
     centerCursor();
     setFocus();

     QTextCursor cursorToPulsate=cursor;
     cursorToPulsate.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);
     pulsate(cursorToPulsate, 700);
 }

 void CodeEditor::pulsate(const QTextCursor &cursor, int duration)
 {
     pulsatePositions.clear();
     pulsatePositions.append(cursor);
     highlightCurrentLine();
     QTimer::singleShot(duration, this, SLOT(removePulsatePositions()));
 }

 void CodeEditor::setErrorPosition(const QTextCursor &cursor)
 {
     this->errorPosition=cursor;
     highlightCurrentLine();

     updateNavBar();
 }
