#include "codeeditor.h"
#include "codeeditordocument.h"
#include "syntaxhighligher.h"
#include "linenumberarea.h"
#include "codecollapsearea.h"
#include "linenavigationbar.h"
#include "blockdata.h"
#include "dialogs/gotolinedialog.h"
#include "util/widgethelper.h"
#include "util/iconutil.h"
#include "util/settings.h"
#include "app_menu/appmenu.h"
#include "app_menu/appeditmenu.h"
#include "code_parser/plsql/plsqlparsehelper.h"
#include "beans/codecollapseposition.h"
#include "widgets/tooltipwidget.h"
#include <QPainter>

#define EDITOR_SEARCH_BG_COLOR QColor(Qt::yellow)
#define EDITOR_ERROR_TEXT_COLOR Qt::red
#define EDITOR_CURR_LINE_NAVBAR_COLOR palette().color(QPalette::Window).darker(130)
#define EDITOR_PULSATE_COLOR (QColor(210,255,165))

QList<CodeEditor*> CodeEditor::openEditors;
QStringHash CodeEditor::textShortcuts;

CodeEditor::CaseFoldingType CodeEditor::keywordCaseFolding;
CodeEditor::CaseFoldingType CodeEditor::identifierCaseFolding;

CodeEditor::CodeEditor(bool plsqlMode, QWidget *parent) :
    QPlainTextEdit(parent),
    lineNumberArea(0),
    lineNavBar(0),
    markedLineIx(-1),
    lineMarkerUsed(false),
    completer(0),
    collapsePositions(0),
    blockEventChanges(false),
    lastParseId(-1)
{
    Q_UNUSED(plsqlMode);

    setDocument(new CodeEditorDocument(this));

    toolTipWidget = new ToolTipWidget(this->viewport());

    QFont monospaceFont("Monospace");
    setFont(monospaceFont);

    lineNumberArea = new LineNumberArea(this);
    codeCollapseArea = new CodeCollapseArea(this);
    codeCollapseArea->installEventFilter(this);
    codeCollapseArea->setMouseTracking(true);
    lineNavBar = new LineNavigationBar(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    //connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateNavBarHighlightColors()));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();

    setTabStopWidth(30);
    setWordWrapMode(QTextOption::NoWrap);
    static_cast<CodeEditorDocument*>(document())->setHighlighter(new SyntaxHighlighter(this->document()));

    viewport()->installEventFilter(this);

    QPalette p=palette();
    p.setColor(QPalette::Disabled, QPalette::Base, p.color(QPalette::Window));
    setPalette(p);

    QFont f;
    f.fromString(Settings::value("CodeEditor/currentFont", WidgetHelper::getMonospaceFont(qApp->font(this).pointSize())).toString());
    setFont(f);

    strTab=QString("   ");


    connect(this, SIGNAL(undoAvailable(bool)), this, SLOT(setUndoAvailable(bool)));
    connect(this, SIGNAL(redoAvailable(bool)), this, SLOT(setRedoAvailable(bool)));

    new QShortcut(QKeySequence(tr("Ctrl+=", "CodeEditor|Increase font size")),
                  this, SLOT(increaseFontSize()), 0, Qt::WidgetWithChildrenShortcut);

    CodeEditor::loadTextShortcuts();
    CodeEditor::openEditors.append(this);
}

CodeEditor::~CodeEditor()
{
    CodeEditor::openEditors.removeOne(this);
}

void CodeEditor::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    Q_UNUSED(queryScheduler);

    Q_ASSERT(this->completer == 0);

    QCompleter *completer = new QCompleter(this);
    setCompleter(completer);
}

//use only when sure that there's not lots of text in editor
QStringList CodeEditor::getSemicolonSeparated() const
{
    return toPlainText().split(';', QString::SkipEmptyParts);
}

int CodeEditor::lineNumberAreaWidth() const
{
     int digits = 1;
     int max = qMax(1, blockCount());
     while (max >= 10) {
         max /= 10;
         ++digits;
     }

     int space = 5 + 5 + fontMetrics().width(QLatin1Char('9')) * digits + lineMarkerAreaWidth(); //area for drawing marker icons

     return space;
}

int CodeEditor::lineMarkerAreaWidth() const
{
    return lineMarkerVisible() ? (fontMetrics().height() + lineMarkerAreaOffset()) : 0;
}

int CodeEditor::lineMarkerAreaOffset() const
{
    return 3;
}

 void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
 {
     bool scrollbarVisible = verticalScrollBar()->isVisibleTo(this);
     lineNavBar->setVisible(scrollbarVisible);
     int rightMargin = scrollbarVisible ? lineNavBar->width() : 0;

     setViewportMargins(lineNumberAreaWidth()+codeCollapseAreaWidth(), 0, rightMargin, 0);
 }



 void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
 {
     if (dy){
         lineNumberArea->scroll(0, dy);
         codeCollapseArea->scroll(0, dy);
     }
     else{
         lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());
         codeCollapseArea->update(0, rect.y(), codeCollapseArea->width(), rect.height());
     }

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

 void CodeEditor::handleTextChanged()
 {
     clearErrorPositions();
     if(lastParseId != -1){
         lastParseId = -1;
         updateNavBar();
     }
 }

 void CodeEditor::clearErrorPositions()
 {
     if(errorPositions.size()>0){
         setErrorPositions(QList<QTextCursor>());
     }
 }

 void CodeEditor::undo()
 {
     this->blockEventChanges = true;

     QPlainTextEdit::undo();

     this->blockEventChanges = false;
 }

 void CodeEditor::redo()
 {
     this->blockEventChanges = true;

     QPlainTextEdit::redo();

     this->blockEventChanges = false;
 }

 void CodeEditor::resizeEvent(QResizeEvent *e)
 {
     QPlainTextEdit::resizeEvent(e);

     QRect cr = contentsRect();
     lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
     codeCollapseArea->setGeometry(QRect(cr.left()+lineNumberAreaWidth(), cr.top(), codeCollapseAreaWidth(), cr.height()));

     QPoint viewportRight = viewport()->mapToParent(viewport()->rect().topRight());
     QRect navBarRect = QRect(viewportRight.x()+1, viewportRight.y(), lineNavBar->width(), viewport()->height());
     lineNavBar->setGeometry(navBarRect);

     //lineNavBar->setGeometry(QRect(cr.right()-lineNavBar->width()+1, cr.top(), lineNavBar->width(), cr.height()));

     if(lineNavBar->isVisible()!=verticalScrollBar()->isVisible()){
         updateLineNumberAreaWidth(0);
     }
 }

 void CodeEditor::highlightCurrentLine()
 {
     QList<QTextEdit::ExtraSelection> extraSelections;

     if (isEnabled()) {
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
         pulsateSelection.format.setBackground(EDITOR_PULSATE_COLOR);
         pulsateSelection.cursor=pulsatePositions.at(i);

         extraSelections.append(pulsateSelection);
     }

     for(int i=0; i<errorPositions.size(); ++i){
         QTextEdit::ExtraSelection errorSelection;
         errorSelection.format.setUnderlineStyle(QTextCharFormat::WaveUnderline);
         errorSelection.format.setUnderlineColor(Qt::red);
         errorSelection.format.setForeground(EDITOR_ERROR_TEXT_COLOR);
         errorSelection.cursor=errorPositions.at(i);

         extraSelections.append(errorSelection);
     }

     if(!collapsibleRegionPositions.isNull()){
         QTextEdit::ExtraSelection collapsibleRegionSelection;
         collapsibleRegionSelection.format.setBackground(EDITOR_PULSATE_COLOR);
         collapsibleRegionSelection.cursor=collapsibleRegionPositions;
         collapsibleRegionSelection.format.setProperty(QTextFormat::FullWidthSelection, true);

         extraSelections.append(collapsibleRegionSelection);
     }

     highlightMatchingBraces(extraSelections);

     setExtraSelections(extraSelections);

     updateNavBar();
 }

 void CodeEditor::highlightMatchingBraces(QList<QTextEdit::ExtraSelection> &extraSelections)
 {
     QTextCursor cur = textCursor();
     int curPos = cur.position();
     int characterCount = document()->characterCount();
     QChar ch = checkForBrace(curPos);

     if(ch.isNull()){
         return;
     }

     if(cur.position() != curPos){
         cur.setPosition(curPos);
     }

     int nextPos;

     if(ch=='('){
         nextPos = findMatchingBrace('(', ')', curPos+1, characterCount-1);
     }else if(ch==')'){
         nextPos = findMatchingBrace(')', '(', curPos-1, 0);
     }

     QTextEdit::ExtraSelection selection1, selection2;

     cur.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
     selection1.cursor = cur;
     selection1.format.setForeground(nextPos==-1 ? Qt::red : Qt::magenta);
     selection1.format.setFontWeight(QFont::Bold);
     extraSelections.append(selection1);

     if(nextPos!=-1){
         QTextCursor nextPosCursor = textCursor();
         nextPosCursor.setPosition(nextPos);
         nextPosCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
         selection2.cursor = nextPosCursor;
         selection2.format.setForeground(Qt::magenta);
         selection2.format.setFontWeight(QFont::Bold);
         extraSelections.append(selection2);
     }
 }

 QChar CodeEditor::checkForBrace(int &pos)
 {
     QChar ch = document()->characterAt(pos);

     if(ch=='(' || ch==')'){
        return ch;
     }else{
         ch = document()->characterAt(--pos);
         if(ch=='(' || ch==')'){
             return ch;
         }/*else{
             pos-=2;
             ch = document()->characterAt(pos);
             if(ch=='(' || ch==')'){
                 return ch;
             }
         }*/
     }

     return QChar();
 }

 int CodeEditor::findMatchingBrace(const QChar &brace, const QChar &matching, int fromPos, int toPos)
 {
     int occurence = 0;
     for(int i=fromPos; toPos>fromPos ? (i<=toPos) : (i>=toPos); toPos>fromPos ? ++i : --i){
         QChar c = document()->characterAt(i);
         if(c == brace){
             --occurence;
         }else if(c == matching){
             if(occurence == 0){
                 return i;
             }
             ++occurence;
         }
     }

     return -1;
 }

 void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
 {
     QPainter painter(lineNumberArea);
     //QRect rect=event->rect();
     //painter.fillRect(rect, palette().color(QPalette::Window));
     painter.setRenderHint(QPainter::Antialiasing);
     //painter.setRenderHint(QPainter::TextAntialiasing);
     QTextBlock block = firstVisibleBlock();
     int blockNumber = block.blockNumber();
     int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
     int bottom = top + (int) blockBoundingRect(block).height();

     while (block.isValid() && top <= event->rect().bottom()) {
         if (block.isVisible() && bottom >= event->rect().top()) {

             if(blockNumber == this->markedLineIx && this->markedLineIx!=-1){
                 int markerAreaOffset = lineMarkerAreaOffset();
                 //int markerAreaCleanWidth = lineMarkerAreaWidth() - markerAreaOffset;

                 float triangleTop = top;
                 float triangleBottom = triangleTop + lineMarkerAreaWidth();
                 float triagleCenter = top + (float)lineMarkerAreaWidth() / 2;
                 float triangleLeft = markerAreaOffset;
                 float triangleRight = lineMarkerAreaWidth();

                 QPolygonF triangle;
                 triangle.append(QPointF(triangleLeft,triangleTop));
                 triangle.append(QPointF(triangleRight, triagleCenter));
                 triangle.append(QPointF(triangleLeft, triangleBottom));
                 triangle.append(QPointF(triangleLeft,triangleTop));

                 QPainterPath paintPath;
                 paintPath.addPolygon(triangle);
                 //QPen pen(Qt::SolidLine);
                 //pen.setJoinStyle(Qt::RoundJoin);
                 painter.setPen(Qt::SolidLine);
                 QLinearGradient gradient(triangleLeft, triangleTop, triangleLeft, triangleBottom);
                 gradient.setColorAt(0.0, Qt::green);
                 gradient.setColorAt(0.7, Qt::darkGreen);
                 painter.setBrush(gradient);
                 painter.drawPath(paintPath);
             }

             QString number = QString::number(blockNumber + 1);
             int offset = lineMarkerAreaWidth();
             painter.drawText(offset, top, lineNumberArea->width()-offset-5, fontMetrics().height(),
                              Qt::AlignRight, number);
         }

         block = block.next();
         top = bottom;
         bottom = top + (int) blockBoundingRect(block).height();
         ++blockNumber;
     }

     //painter.setPen(Qt::DotLine);
     //painter.drawLine(rect.right(), rect.top(), rect.right(), rect.bottom());
 }

 void CodeEditor::codeCollapseAreaPaintEvent(QPaintEvent *event)
 {
     QPainter painter(codeCollapseArea);

     int selectionStart = collapsibleRegionPositions.isNull() ? -1 : collapsibleRegionPositions.selectionStart();
     int selectionEnd = collapsibleRegionPositions.isNull() ? -1 : collapsibleRegionPositions.selectionEnd();

     QTextBlock block = firstVisibleBlock();
     int blockNumber = block.blockNumber();
     int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
     int bottom = top + (int) blockBoundingRect(block).height();

     QStyleOption option;
     option.initFrom(codeCollapseArea);

     while (block.isValid() && top <= event->rect().bottom()) {
         if (block.isVisible() && bottom >= event->rect().top()) {

             BlockData *data = static_cast<BlockData*>(block.userData());
             if(data){

                 bool insideHighlightArea = (selectionStart!=-1 && block.position()>=selectionStart && block.position()<=selectionEnd);

                 if(insideHighlightArea){
                     option.rect = QRect(0,top,codeCollapseAreaWidth(),bottom-top);
                     painter.fillRect(option.rect, EDITOR_PULSATE_COLOR);
                 }

                 if(data->isCollapsePosition(lastParseId)){
                     option.rect = QRect(0,top,codeCollapseAreaWidth()-5,bottom-top);
                     if(insideHighlightArea && block.blockNumber() == collapsibleRegionStartBlockNumber){
                        option.state = QStyle::State_Children  | QStyle::State_Active | QStyle::State_MouseOver | QStyle::State_Enabled;
                     }else{
                        option.state = QStyle::State_Children;
                     }

                     if(data->getCollapsedUntil()==-1){
                         option.state |= QStyle::State_Open;
                     }

                     qApp->style()->drawPrimitive(QStyle::PE_IndicatorBranch, &option, &painter);
                 }
             }
         }

         block = block.next();
         top = bottom;
         bottom = top + (int) blockBoundingRect(block).height();
         ++blockNumber;
     }
 }

 int CodeEditor::codeCollapseAreaWidth() const
 {
     return codeCollapseArea ? 16 : 0;
 }

 void CodeEditor::refreshCodeCollapseArea()
 {
     codeCollapseArea->update();
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

     int currentBlockNo = textCursor().blockNumber();
     bool hasHighlightOnCurrentBlock = false;

     for(int i=0; i<foundTextPositions.size(); ++i){
         const QTextCursor &cur=foundTextPositions.at(i);
         Q_ASSERT(!cur.isNull());
         drawLineNavBarHighlight(painter, cur.blockNumber(), EDITOR_SEARCH_BG_COLOR, docHeight, blockHeight, hasScrollbars);

         if(cur.blockNumber()==currentBlockNo && !hasHighlightOnCurrentBlock){
             hasHighlightOnCurrentBlock=true;
         }
     }

     for(int i=0; i<errorPositions.size(); ++i){
         const QTextCursor &cur=errorPositions.at(i);
         Q_ASSERT(!cur.isNull());
         drawLineNavBarHighlight(painter, cur.blockNumber(), EDITOR_ERROR_TEXT_COLOR, docHeight, blockHeight, hasScrollbars);

         if(cur.blockNumber()==currentBlockNo && !hasHighlightOnCurrentBlock){
             hasHighlightOnCurrentBlock=true;
         }
     }

     //highlight current line
     drawLineNavBarHighlight(painter,
                             currentBlockNo,
                             EDITOR_CURR_LINE_NAVBAR_COLOR,
                             docHeight, blockHeight, hasScrollbars,
                             !hasHighlightOnCurrentBlock);
 }

 void CodeEditor::drawLineNavBarHighlight(QPainter &painter,
                                          int blockNumber,
                                          const QColor &color,
                                          qreal docHeight,
                                          qreal blockHeight,
                                          bool hasScrollbars,
                                          bool fullLength)
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

     int xPos = fullLength ? 1 : qCeil(lineNavBar->width()/2);
     int width = fullLength ? lineNumberArea->width()-1 : qCeil(lineNumberArea->width()-1/2)-1;
     painter.fillRect(QRect(xPos,(int)highlightTop,width,(int)highlightHeight), color);
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

     updateNavBar();
 }

 QList<int> CodeEditor::getHighlightedBlockNumbers() const
 {
     QList<int> results;
     results.reserve(foundTextPositions.size()+errorPositions.size());

     for(int i=0; i<foundTextPositions.size(); ++i){
         const QTextCursor &cur=foundTextPositions.at(i);
         results.append(cur.blockNumber());
     }

     for(int i=0; i<errorPositions.size(); ++i){
         const QTextCursor &cur=errorPositions.at(i);
         results.append(cur.blockNumber());
     }

     return results;
 }

 bool CodeEditor::lineMarkerVisible() const
 {
     return markedLineIx!=-1 || lineMarkerUsed;
 }

 /*
 void CodeEditor::lineNumberAreaWheelEvent(QWheelEvent *event)
 {
     this->wheelEvent(event);
 }*/

 void CodeEditor::lineNavBarWheelEvent(QWheelEvent *event)
 {
     this->wheelEvent(event);
 }

 void CodeEditor::keyReleaseEvent ( QKeyEvent * event )
 {
     QPlainTextEdit::keyReleaseEvent(event);

     if(event->key()==Qt::Key_Control){
         event->ignore();
     }else if(event->key()==Qt::Key_Escape){
        emit escapeKeyPressed();
     }
 }

 void CodeEditor::keyPressEvent ( QKeyEvent * event )
 {
     bool handled=false;

     expandAllBlocks(event);

     if(!isReadOnly()){
         int key=event->key();

         if(completer && completer->popup()->isVisible()){
             switch (key) {
             case Qt::Key_Enter:
             case Qt::Key_Return:
             case Qt::Key_Escape:
             case Qt::Key_Tab:
             case Qt::Key_Backtab:
                 event->ignore();
                 return; // let the completer do default behavior
             default:
                 break;
             }
         }else if((key==Qt::Key_Return || key==Qt::Key_Enter) && event->modifiers()==0){
            handled=true;
            textCursor().beginEditBlock();
            QPlainTextEdit::keyPressEvent(event);
            autoIndentNewBlock();
            textCursor().endEditBlock();
         }else if(key==Qt::Key_Tab){
             handled=true;

             if((event->modifiers() & Qt::ControlModifier)==Qt::ControlModifier){
                 event->setAccepted(false);
             }else{
                 if(textCursor().hasSelection()){
                     if(event->modifiers() & Qt::ShiftModifier){
                         unindentSelection();
                     }else{
                         indentSelection();
                     }
                 }else{
                     insertTab();
                 }
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
         }else if(event->matches(QKeySequence::Undo)){
             handled=true;
             undo();
         }else if(event->matches(QKeySequence::Redo)){
             handled=true;
             redo();
         }

         if(!handled && key!=Qt::Key_Backspace && completer){
             completerHandleKeyPress(event);
             handled = true;
         }
     }

     if(!handled){
        QPlainTextEdit::keyPressEvent(event);
     }
 }

 void CodeEditor::completerHandleKeyPress(QKeyEvent *event)
 {
     bool isShortcut = ((event->modifiers() & Qt::ControlModifier) && event->key() == Qt::Key_Space); // CTRL+Space
     if (!isShortcut){ // do not process the shortcut when we have a completer}
        QPlainTextEdit::keyPressEvent(event);
     }

     const bool ctrlOrShift = event->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
     if (ctrlOrShift && event->text().isEmpty()){
         return;
     }

     static QString eow("~!@#%^&*_+{}|:\"<>?,/;'[]\\-=$"); // end of word
     bool hasModifier = (event->modifiers() != Qt::NoModifier) && !ctrlOrShift;
     QString completionPrefix = textUnderCursor();
     //completionPrefix.append(event->text());

     static QString triggers("."); //characters to start completer
     bool startTriggerPressed = !event->text().isEmpty() && triggers.contains(event->text().right(1));

     if (!isShortcut && (hasModifier || event->text().isEmpty()|| completionPrefix.length() < 1
                         || eow.contains(event->text().right(1))) && !startTriggerPressed) {
         completer->popup()->hide();
         return;
     }


     if(!completer->popup()->isVisible() && startTriggerPressed){
         emit needsCompletionList();
         return;
     }else if(completer->popup()->isVisible()){
         if (completionPrefix != completer->completionPrefix()) {
             completer->setCompletionPrefix(completionPrefix);
             completer->popup()->setCurrentIndex(completer->completionModel()->index(0, 0));
         }
         QRect cr = cursorRect();
         cr.setWidth(completer->popup()->sizeHintForColumn(0)
                     + completer->popup()->verticalScrollBar()->sizeHint().width());
         completer->complete(cr); // popup it up!
     }
 }

 void CodeEditor::insertCompletion(const QString &completion)
 {
     if (completer->widget() != this){
         return;
     }

     QString prefix = completer->completionPrefix();

     QTextCursor tc = textCursor();
     int extra = completion.length() - prefix.length();
     tc.movePosition(QTextCursor::Left);
     tc.movePosition(QTextCursor::EndOfWord);

     QString textToInsert = completion.right(extra);

     if(prefix.size()>0){
         if(prefix.at(prefix.size()-1).isLower()){
             textToInsert = textToInsert.toLower();
         }
     }

     tc.insertText(textToInsert);
     setTextCursor(tc);
 }
/*
 void CodeEditor::completionModelReady(QAbstractItemModel *model, int cursorPosition)
 {
     //set model
     QAbstractItemModel *currentModel = completer->model();
     completer->setModel(model);
     delete currentModel;

     if(model!=0){
         QRect cr = cursorRect();
         cr.setWidth(completer->popup()->sizeHintForColumn(0)
                     + completer->popup()->verticalScrollBar()->sizeHint().width());
         completer->complete(cr); // popup it up!
     }
 }*/

 CodeCollapsePosition CodeEditor::findCollapsePosition(const QTextBlock &block)
 {
     int blockNumber = block.blockNumber();

     if(blockNumber<0 || blockNumber >= blockCount()){
         return CodeCollapsePosition();
     }

     int scannedCount = 0;

     QTextBlock prevBlock = block;
     while(prevBlock.isValid() && ++scannedCount<200){

         BlockData *data = static_cast<BlockData*>(prevBlock.userData());
         if(!data){
             return CodeCollapsePosition();
         }

         int collapseEndLine = data->getCollapseEnd(lastParseId);
         if(collapseEndLine!=-1 && collapseEndLine>=blockNumber){
             return CodeCollapsePosition(prevBlock.blockNumber(), collapseEndLine);
         }

         prevBlock = prevBlock.previous();
     }

     return CodeCollapsePosition();
 }

 void CodeEditor::highlightCollapsibleRegion(QMouseEvent *event)
 {
     QTextCursor cur = cursorForPosition(event->pos());
     if(!cur.isNull()){
         highlightCollapsibleRegion(cur.block());
     }
 }

 void CodeEditor::highlightCollapsibleRegion(const QTextBlock &block)
 {
     CodeCollapsePosition collapsePos=findCollapsePosition(block);

     if(!collapsePos.isValid()){
         collapsibleRegionPositions = QTextCursor();
         return;
     }

     QTextCursor cur = textCursor();
     QTextBlock startBlock=document()->findBlockByNumber(collapsePos.startLine);

     if(startBlock.position() == collapsibleRegionPositions.selectionStart()){ //already highlighted
         return;
     }

     QTextBlock endBlock=document()->findBlockByNumber(collapsePos.endLine);

     if(!startBlock.isValid() || !endBlock.isValid()){
         return;
     }

     selectBlocks(cur, startBlock, endBlock);

     collapsibleRegionPositions = cur;
     collapsibleRegionStartBlockNumber = startBlock.blockNumber();

     highlightCurrentLine();
 }

 void CodeEditor::selectBlocks(QTextCursor &cur, const QTextBlock &startBlock, const QTextBlock &endBlock)
 {
     cur.setPosition(startBlock.position());
     cur.setPosition(endBlock.position(), QTextCursor::KeepAnchor);
     cur.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
 }

 void CodeEditor::collapseOrExpandBlocks(QTextCursor &cur)
 {
     setUpdatesEnabled(false);
     this->blockEventChanges=true;

     QTextBlock startBlock = document()->findBlock(cur.selectionStart());
     Q_ASSERT(startBlock.isValid());

     BlockData *data = static_cast<BlockData*>(startBlock.userData());
     Q_ASSERT(data);

     int collapsedUntil = data->getCollapsedUntil();
     bool collapsing = collapsedUntil == -1;


     int endBlockNumber = collapsing ? document()->findBlock(cur.selectionEnd()).blockNumber()
                                     :  collapsedUntil;

     Q_ASSERT(endBlockNumber!=-1);

     data->setCollapsedUntil(collapsing ? endBlockNumber : -1);
     //must be called after marking block as parent block of collapsed region
     //static_cast<CodeEditorDocument*>(this->document())->forceSyntaxRehighlighting(startBlock.position()+1);
     static_cast<CodeEditorDocument*>(document())->forceSyntaxRehighlighting(startBlock);

     while(startBlock.blockNumber() < endBlockNumber && startBlock.isValid()) {
         startBlock = startBlock.next(); //do not hide/show first block

         if(!collapsing){ //do not expand nested collapsed regions
            data = static_cast<BlockData*>(startBlock.userData());
            Q_ASSERT(data);

            int innerCollapsedUntil = data->getCollapsedUntil();
            if(innerCollapsedUntil !=-1){
                startBlock.setVisible(true); //expand first line of collapsed region
                while(startBlock.blockNumber() < innerCollapsedUntil && startBlock.isValid()){ //skip nested collapsed blocks
                    startBlock = startBlock.next();
                }
                continue;
            }
         }

         startBlock.setVisible(!collapsing);
     }

     CodeEditorDocument *doc = static_cast<CodeEditorDocument*>(document());
     if(collapsing){
         doc->incrementCollapsedRangeCount();
     }else{
         doc->decrementCollapsedRangeCount();
     }

     this->blockEventChanges=false;
     setUpdatesEnabled(true);

     document()->markContentsDirty(0,document()->characterCount());

     updateAllParts();

     emit updated(this);
 }

 void CodeEditor::showTooltipForCollapsedRange(const QTextBlock &block, int endBlockNumber)
 {
     Q_ASSERT(block.isValid());

     QTextBlock endBlock = document()->findBlockByNumber(endBlockNumber);

     Q_ASSERT(endBlock.isValid() && block < endBlock);

     QTextCursor cur = textCursor();
     cur.setPosition(block.position());
     cur.movePosition(QTextCursor::NextBlock);
     cur.setPosition(endBlock.position()+endBlock.length()-1, QTextCursor::KeepAnchor);
     QTextDocumentFragment fragment(cur);
     QString html = fragment.toHtml();

     QRect blockRect = blockBoundingGeometry(block).translated(contentOffset()).toRect();

     toolTipWidget->setFont(this->font());
     toolTipWidget->setText(html);
     toolTipWidget->showToolTip(viewport()->mapToGlobal(blockRect.bottomLeft()),
                                blockRect);
 }

 int CodeEditor::collapsedRangeCount() const
 {
     return static_cast<CodeEditorDocument*>(document())->getCollapsedRangeCount();
 }

 bool CodeEditor::extendSelection(QTextCursor &cur)
 {
     if(!cur.hasSelection() || collapsedRangeCount()==0){
         return false;
     }

     CursorPositionInfo cursorInfo = getStartStopPositions(cur);

     QTextBlock block = document()->findBlock(cursorInfo.endPos);
     Q_ASSERT(block.isValid());

     if(cursorInfo.endPos == (block.position()+block.length()-1)){ //block selected until end
         BlockData *data = static_cast<BlockData*>(block.userData());
         if(data->isCollapsedRangeStart()){
             QTextBlock collapseEndBlock = document()->findBlockByNumber(data->getCollapsedUntil());
             Q_ASSERT(collapseEndBlock.isValid());

             cursorInfo.endBlock = collapseEndBlock.blockNumber();
             cursorInfo.endPos = collapseEndBlock.position() + collapseEndBlock.length() - 1;

             cursorInfo.selectText(cur);

             collapseOrExpandBlocks(cur);

             return true;
         }
     }

     return false;
 }

 void CodeEditor::expandAllBlocks(int startBlock, int endBlock)
 {
     if(collapsedRangeCount()==0){
         return;
     }

     QTextBlock block = document()->findBlockByNumber(startBlock);
     while(block.isValid() && block.blockNumber() <= endBlock){
         expandBlock(block);

         block = block.next();
     }
 }

 void CodeEditor::expandAllBlocks(QKeyEvent *event)
 {
     if(collapsedRangeCount()==0){
         return;
     }

     if(!event->matches(QKeySequence::Copy) &&
             !event->matches(QKeySequence::Cut) &&
             (event->key()!=Qt::Key_Up) &&
             (event->key()!=Qt::Key_Down) &&
             (event->key()!=Qt::Key_Left) &&
             (event->key()!=Qt::Key_Right) &&
             (event->key()!=Qt::Key_PageUp) &&
             (event->key()!=Qt::Key_PageDown) &&
             (event->key()!=Qt::Key_Control) &&
             (event->key()!=Qt::Key_Meta) &&
             (event->key()!=Qt::Key_Alt) &&
             (event->key()!=Qt::Key_AltGr)){
         CursorPositionInfo inf = getStartStopPositions(textCursor());
         expandAllBlocks(inf.startBlock, inf.endBlock);
     }
 }

 void CodeEditor::expandBlock(const QTextBlock &block)
 {
     if(collapsedRangeCount()==0){
         return;
     }

     QTextBlock currBlock = block;
     while(!currBlock.isVisible() && currBlock.isValid()){
         currBlock = currBlock.previous();
     }

     BlockData *data = static_cast<BlockData*>(currBlock.userData());
     if(data->isCollapsedRangeStart()){
         QTextBlock collapseEndBlock = document()->findBlockByNumber(data->getCollapsedUntil());
         Q_ASSERT(collapseEndBlock.isValid());

         QTextCursor cur = textCursor();
         cur.setPosition(currBlock.position());
         cur.setPosition(collapseEndBlock.position()+collapseEndBlock.length()-1, QTextCursor::KeepAnchor);

         collapseOrExpandBlocks(cur);
     }
 }

 void CodeEditor::updateAllParts()
 {
     viewport()->update();
     lineNumberArea->update();
     codeCollapseArea->update();
     lineNavBar->update();
     update();
 }

 void CodeEditor::setDocument(QTextDocument *document)
 {
     QPlainTextEdit::setDocument(document);

     connect(this->document(), SIGNAL(contentsChange(int,int,int)), this, SLOT(handleTextChanged()));
 }

 void CodeEditor::focusInEvent(QFocusEvent *event)
 {
     if (completer){
         completer->setWidget(this);
     }

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
     //menu->addAction(editMenu->editCopyAsAction);
     menu->addAction(editMenu->editPasteAction);
     menu->addSeparator();
     menu->addAction(editMenu->editSelectAllAction);
     menu->addSeparator();
     menu->addAction(editMenu->editAdvancedAction);
     menu->addSeparator();
     menu->addAction(editMenu->editDescribeAction);
     menu->addAction(editMenu->editResolveAction);

     menu->exec(event->globalPos());
     delete menu;
 }

 bool CodeEditor::event(QEvent *e)
 {
     if(e->type()==QEvent::FontChange && lineNumberArea){
         QFont newFont=this->font();

         lineNumberArea->setFont(newFont);
     }

     return QPlainTextEdit::event(e);
 }

 void CodeEditor::paintEvent(QPaintEvent *event)
 {
     QPlainTextEdit::paintEvent(event);

     QPainter painter(viewport());

     QTextBlock block = firstVisibleBlock();
     int blockNumber = block.blockNumber();
     QRectF blockGeometry = blockBoundingGeometry(block).translated(contentOffset());
     int top = (int) blockGeometry.top();
     int bottom = top + (int) blockBoundingRect(block).height();

     /*QTextCursor cur = textCursor();
     QFontMetrics metrics = fontMetrics();
     int charWidth = metrics.width('W');
     int charHeight = metrics.height();*/

     painter.save();
     painter.setPen(QPen(Qt::red, 0.5));

     while (block.isValid() && top <= event->rect().bottom()) {
         if (blockNumber > 0 && block.isVisible() && bottom >= event->rect().top()) {

             BlockData *data = static_cast<BlockData*>(block.userData());
             if(data){
                 if(data->isSectionSeparator()){
                     //painter.setPen(QPen(Qt::red, 0.5));
                     //painter.setRenderHint(QPainter::Antialiasing, false);
                     painter.drawLine(0, top, viewport()->width(), top);
                 }
                 /*if(data->isCollapsedRangeStart()){
                     painter.setPen(QPen(palette().text(), 0.3));
                     painter.setRenderHint(QPainter::Antialiasing, true);
                     cur.setPosition(block.position()+block.length()-1);
                     QRect curRect = cursorRect(cur).translated(contentOffset().toPoint()).translated(10, 0);
                     QRect blockEndRect(curRect.left(), top+(qMax((bottom-top-charHeight),0)/(float)2), charWidth*5, charHeight);
                     painter.drawRoundedRect(blockEndRect, 10, 10, Qt::RelativeSize);
                 }*/
             }

         }

         block = block.next();
         top = bottom;
         bottom = top + (int) blockBoundingRect(block).height();
         ++blockNumber;
     }
     painter.restore();
 }

 void CodeEditor::autoIndentNewBlock()
 {
     this->blockEventChanges = true;

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

     this->blockEventChanges = false;
 }

 void CodeEditor::indentSelection()
 {
     QTextCursor cur=textCursor();
     if(!cur.hasSelection()){
         return;
     }

     this->blockEventChanges = true;

     cur.beginEditBlock();
     CursorPositionInfo inf=getStartStopPositions(cur);

     cur.setPosition(inf.startPos);
     cur.movePosition(QTextCursor::StartOfBlock);
     int blocksToMove=(inf.endBlock-inf.startBlock)+1;
     QString space(' ');
     for(int i=0; i<blocksToMove; ++i){
        cur.insertText(space);
        cur.movePosition(QTextCursor::NextBlock);
     }
     cur.endEditBlock();

     inf.startPos+=space.size();
     inf.endPos+=blocksToMove*space.size();
     inf.selectText(cur);

     setTextCursor(cur);

     this->blockEventChanges = false;
 }

 void CodeEditor::unindentSelection()
 {
     QTextCursor cur=textCursor();
     if(!cur.hasSelection()){
         return;
     }

     this->blockEventChanges = true;

     cur.beginEditBlock();
     CursorPositionInfo inf=getStartStopPositions(cur);

     cur.setPosition(inf.startPos);
     //cur.movePosition(QTextCursor::StartOfBlock);
     int blocksToMove=(inf.endBlock-inf.startBlock)+1;
     for(int i=0; i<blocksToMove; ++i){
        int posInBlockBeforeMove = i==0 ? cur.positionInBlock() : -1; //needed only for first block
        moveToFirstNonSpaceCharacter(cur);
        //for(int k=0; k<strTab.size(); ++k){
            if(cur.positionInBlock()>0){
                cur.deletePreviousChar();
                if(i==0 && posInBlockBeforeMove > 0){
                    --inf.startPos;
                }

                --inf.endPos;
            }else{
                break;
            }
        //}

        cur.movePosition(QTextCursor::NextBlock);
     }
     cur.endEditBlock();
     inf.selectText(cur);
     setTextCursor(cur);

     this->blockEventChanges = false;
 }

 void CodeEditor::insertTab()
 {
     QTextCursor cur = textCursor();
     Q_ASSERT(!cur.hasSelection()); //this function is called only when there is no selection

     if(cur.atBlockStart()){
         cur.insertText(strTab);
         return;
     }

     bool insertedTextShortcut = insertTextShortcut(cur);

     if(!insertedTextShortcut){
        textCursor().insertText(strTab);
     }
 }

 bool CodeEditor::insertTextShortcut(QTextCursor &cur)
 {
     QList<QString> keys = CodeEditor::textShortcuts.keys();
     for(int i=0; i<keys.size(); ++i){
         const QString &key = keys.at(i);
         int posToSet = cur.position() - key.size();
         if(posToSet<0){
             continue;
         }
         cur.setPosition(posToSet, QTextCursor::KeepAnchor);
         if(cur.selectedText().compare(key, Qt::CaseInsensitive) == 0){
             QString value = CodeEditor::textShortcuts[key];

             int cursorPos = value.length();
             //check if value contains |
             int pipePos = value.lastIndexOf('|');
             if(pipePos!=-1 && value.length()==1){
                 return false;
             }

             while(pipePos != -1){
                 if((pipePos==value.length()-1 && value.at(pipePos-1)=='|') ||
                         (pipePos==0 && value.at(1)=='|') ||
                         (pipePos!=value.length()-1 && pipePos>0 && (value.at(pipePos-1)=='|' || value.at(pipePos+1)=='|'))){
                     pipePos = value.lastIndexOf('|', pipePos-2);
                 }else{
                     value.remove(pipePos, 1);
                     cursorPos = value.length() - pipePos;
                     break;
                 }
             }

             cur.insertText(value);
             if(cursorPos != value.length()){
                 cur.setPosition(cur.position() - cursorPos);
                 setTextCursor(cur);
             }
             return true;
         }else{
             cur.setPosition(posToSet + key.size());
         }
     }

     return false;
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
     }else{
         extendSelection(cur);
         setTextCursor(cur);
     }
 }


 void CodeEditor::updateNavBar()
 {
     lineNavBar->update();
     codeCollapseArea->update();
 }

 void CodeEditor::commentBlocks()
 {
     this->blockEventChanges = true;

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

     this->blockEventChanges = false;
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

    applyCurrentFontToAllEditors();
 }

 void CodeEditor::decreaseFontSize()
 {
     WidgetHelper::changeFontSize(this, -0.5);

     applyCurrentFontToAllEditors();
 }

 void CodeEditor::resetFontSize()
 {
     this->setFont(WidgetHelper::getMonospaceFont(qApp->font(this).pointSize()));

     applyCurrentFontToAllEditors();
 }

 void CodeEditor::applyCurrentFontToAllEditors()
 {
     foreach(CodeEditor *e, CodeEditor::openEditors){
         if(e!=this){
             e->setFont(this->font());
         }
     }

     saveFontSettings();
 }

 void CodeEditor::saveFontSettings()
 {
     Settings::setValue("CodeEditor/currentFont", this->font().toString());
 }

 void CodeEditor::toUpperCase()
 {
     this->blockEventChanges = true;

     changeCase(true);

     this->blockEventChanges = false;
 }

 void CodeEditor::toLowerCase()
 {
     this->blockEventChanges = true;

     changeCase(false);

     this->blockEventChanges = false;
 }

 void CodeEditor::makeDuplicate()
 {
     this->blockEventChanges = true;

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

     this->blockEventChanges = false;
 }

 void CodeEditor::removeEmptyLines()
 {
     this->blockEventChanges = true;

     QTextCursor cur = textCursor();
     int startBlock, endBlock;
     if(cur.hasSelection()){
         CursorPositionInfo info=getStartStopPositions(cur);
         startBlock = info.startBlock;
         endBlock = info.endBlock;
     }else{
         startBlock = 0;
         endBlock = blockCount() - 1;
     }

     cur.beginEditBlock();
     QTextBlock block = document()->findBlockByNumber(startBlock);
     while(block.isValid() && block.blockNumber()<=endBlock){
         cur.setPosition(block.position());
         block = block.next();
         cur.select(QTextCursor::BlockUnderCursor);
         if(cur.selectedText().trimmed().isEmpty()){

             if(cur.movePosition(QTextCursor::PreviousBlock)){
                 cur.movePosition(QTextCursor::EndOfBlock);
                 cur.movePosition(QTextCursor::NextBlock, QTextCursor::KeepAnchor);
             }

             cur.removeSelectedText();
         }
     }
     cur.endEditBlock();

     this->blockEventChanges = false;
 }

 void CodeEditor::moveUp()
 {
    this->blockEventChanges = true;

    moveSelectedText(true);

    this->blockEventChanges = false;
 }

 void CodeEditor::moveDown()
 {
     this->blockEventChanges = true;

     moveSelectedText(false);

     this->blockEventChanges = false;
 }

 void CodeEditor::moveSelectedText(bool up)
 {
     QTextCursor cur=textCursor();
     CursorPositionInfo inf=getStartStopPositions(cur);

     if((up && inf.startBlock==0) || (!up && inf.endBlock>document()->blockCount()-2)){
        return;
     }

     expandAllBlocks(inf.startBlock, inf.endBlock);

     cur.beginEditBlock();

     if(up){
        int startBlockPos=document()->findBlockByNumber(inf.startBlock).position();
        cur.setPosition(startBlockPos);

        cur.movePosition(QTextCursor::PreviousBlock);
        cur.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);

        QTextBlock prevBlock = cur.block();
        expandBlock(prevBlock);

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

         QTextBlock nextBlock = cur.block();
         if(nextBlock.isValid()){
             expandBlock(nextBlock);
         }

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

 void CodeEditor::selectCurrentBlock()
 {
     QTextCursor cur = textCursor();

     CodeCollapsePosition collapsePos=findCollapsePosition(cur.block());
     if(!collapsePos.isValid()){
         return;
     }

     QTextBlock startBlock=document()->findBlockByNumber(collapsePos.startLine);
     QTextBlock endBlock=document()->findBlockByNumber(collapsePos.endLine);
     selectBlocks(cur, startBlock, endBlock);

     setTextCursor(cur);
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

 QString CodeEditor::getCurrentText(QTextCursor &txtCursor, bool fullTextOnNoSelection) const
 {
     QString result;

     QTextCursor cursor=textCursor();
     if(cursor.hasSelection()){
         result = cursor.selectedText().replace(QChar(0x2029), QChar('\n'));
     }else{
         if(!fullTextOnNoSelection){
            result = getCurrentTextSurroundedByEmptyLines(cursor);

            if(!result.isEmpty()){
                int queryStartPos, queryEndPos;
                PlSqlParseHelper::getNextQueryPos(result, 0, &queryStartPos, &queryEndPos);

                if(queryStartPos!=-1 && queryEndPos!=-1){
                    result = result.mid(queryStartPos, queryEndPos-queryStartPos-1);
                    cursor.setPosition(cursor.selectionStart() + queryStartPos, QTextCursor::MoveAnchor);
                    cursor.setPosition(cursor.position() + queryEndPos-queryStartPos-1, QTextCursor::KeepAnchor);
                }
            }
         }else{
            result = this->toPlainText();
            cursor.movePosition(QTextCursor::Start);
         }
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
 }

 void CodeEditor::clearFoundTextPositions()
 {
     if(this->foundTextPositions.size()>0){
        this->foundTextPositions.clear();
        highlightCurrentLine();
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
     Q_ASSERT(!cursor.isNull());

     this->errorPositions.clear();
     setErrorPositions(QList<QTextCursor>() << cursor);
 }

 void CodeEditor::setErrorPositions(const QList<QTextCursor> &errorPositions)
 {
     this->errorPositions = errorPositions;

     highlightCurrentLine();
 }

 void CodeEditor::addErrorPosition(const QTextCursor &cursor)
 {
     Q_ASSERT(!cursor.isNull());

     this->errorPositions.append(cursor);
     highlightCurrentLine();
 }

 void CodeEditor::setMarkedLine(int line)
 {
     if(line == markedLineIx){
         return;
     }

     markedLineIx = line;

     if(line==-1){
         lineNumberArea->update();
     }else{
         Q_ASSERT(line>=0 && line<blockCount());

         if(!lineMarkerUsed){
             lineMarkerUsed=true;
             updateLineNumberAreaWidth(0);
         }else{
             lineNumberArea->update();
         }
     }
 }

 void CodeEditor::setCompleter(QCompleter *completer)
 {
     if (this->completer){
         QObject::disconnect(this->completer, 0, this, 0);
     }

     this->completer = completer;

     if (!completer)
         return;

     this->completer->setWidget(this);
     this->completer->setCompletionMode(QCompleter::PopupCompletion);
     this->completer->setCaseSensitivity(Qt::CaseInsensitive);
     this->completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
     QObject::connect(this->completer, SIGNAL(activated(QString)),
                      this, SLOT(insertCompletion(QString)));
 }

 void CodeEditor::setCodeCollapsePositions(QList<CodeCollapsePosition *> *collapsePositions)
 {
     this->collapsePositions = collapsePositions;
 }

 bool CodeEditor::eventFilter(QObject *watched, QEvent *event)
 {
     /*if(watched == codeCollapseArea){
         if(event->type() == QEvent::ToolTip){
             QHelpEvent *helpEvent = static_cast<QHelpEvent*>( event );

             QTextCursor cur = cursorForPosition(helpEvent->pos());
             highlightCollapsibleRegion(cur.blockNumber());
         }else if(event->type() == QEvent::Leave && !collapsibleRegionPositions.isNull()){
             collapsibleRegionPositions = QTextCursor();
             highlightCurrentLine();
         }
     }*/

     if(watched == codeCollapseArea){
         if(event->type() == QEvent::MouseMove){
             highlightCollapsibleRegion(static_cast<QMouseEvent*>(event));
         }else if(event->type() == QEvent::Leave && !collapsibleRegionPositions.isNull()){
             collapsibleRegionPositions = QTextCursor();
             highlightCurrentLine();
         }else if(event->type() == QEvent::MouseButtonRelease){
             if(!collapsibleRegionPositions.isNull()){
                 collapseOrExpandBlocks(collapsibleRegionPositions);
                 collapsibleRegionPositions = QTextCursor();

                 highlightCollapsibleRegion(static_cast<QMouseEvent*>(event));
             }
         }
     }else if(watched == viewport()){
         if(event->type() == QEvent::ToolTip){
             QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);
             QTextCursor cur = cursorForPosition(helpEvent->pos());
             if(!cur.isNull()){
                 BlockData *data = static_cast<BlockData*>(cur.block().userData());
                 if(data->isCollapsedRangeStart()){
                     showTooltipForCollapsedRange(cur.block(), data->getCollapsedUntil());
                 }
             }
         }
     }

     return QPlainTextEdit::eventFilter(watched, event);
 }

 bool CodeEditor::blockChanges() const
 {
     return this->blockEventChanges;
 }

 void CodeEditor::setBlockChanges(bool block)
 {
     this->blockEventChanges = block;
 }

 void CodeEditor::setLastParseId(int parseId)
 {
     this->lastParseId = parseId;
     updateNavBar();
 }

 int CodeEditor::getLastParseId() const
 {
     return this->lastParseId;
 }

 void CodeEditor::loadTextShortcuts()
 {
     if(!CodeEditor::textShortcuts.isEmpty()){
         return;
     }

     CodeEditor::textShortcuts = Settings::loadStringHash("CodeEditor/shortcuts");
     if(!CodeEditor::textShortcuts.isEmpty()){
         return;
     }

     CodeEditor::textShortcuts["sf"] = "select * from ";
     CodeEditor::textShortcuts["sc"] = "select count(0) from ";
     CodeEditor::textShortcuts["se"] = "select t.*, t.rowid from | t";
     CodeEditor::textShortcuts["ii"] = "insert into ";
     CodeEditor::textShortcuts["df"] = "delete from ";
 }

 QString CodeEditor::textUnderCursor() const
 {
     QTextCursor tc = textCursor();
     tc.select(QTextCursor::WordUnderCursor);
     return tc.selectedText();
 }
