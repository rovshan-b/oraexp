#include "codeeditor.h"
#include "syntaxhighligher.h"
#include "linenumberarea.h"
#include <QPainter>

CodeEditor::CodeEditor(QWidget *parent) :
    QPlainTextEdit(parent)
{
    QFont monospaceFont("Monospace");
    setFont(monospaceFont);

    lineNumberArea = new LineNumberArea(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
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
     setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
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



 void CodeEditor::resizeEvent(QResizeEvent *e)
 {
     QPlainTextEdit::resizeEvent(e);

     QRect cr = contentsRect();
     lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
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
         searchTextSelection.format.setBackground(palette().color(QPalette::Highlight).lighter());
         searchTextSelection.format.setForeground(palette().color(QPalette::HighlightedText));
         searchTextSelection.cursor=foundTextPositions.at(i);

         extraSelections.append(searchTextSelection);
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

 void CodeEditor::keyReleaseEvent ( QKeyEvent * event )
 {
     QPlainTextEdit::keyReleaseEvent(event);

     if(event->key()==Qt::Key_Escape){
        emit escapeKeyPressed();
     }
 }

 void CodeEditor::addText(const QString &text)
 {
     QTextCursor cur=this->textCursor();
     if(!cur.atEnd()){
         cur.movePosition(QTextCursor::End);
     }
     cur.insertText(text);
 }

 QString CodeEditor::getCurrentText() const
 {
     QTextCursor cursor=textCursor();
     if(cursor.hasSelection()){
         return cursor.selectedText().replace(QChar(0x2029), QChar('\n'));
     }else{
         return toPlainText();
     }
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
