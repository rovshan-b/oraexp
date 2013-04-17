#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include "cursorpositioninfo.h"

class LineNavigationBar;

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    CodeEditor(QWidget *parent = 0);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth() const;
    void lineNumberAreaWheelEvent(QWheelEvent * event);

    void lineNavBarPaintEvent(QPaintEvent *event);
    void lineNavBarMouseReleaseEvent(QMouseEvent * event);
    void lineNavBarWheelEvent(QWheelEvent * event);

    //use only when sure that there's not lots of text in editor
    QStringList getSemicolonSeparated() const;

    void addText(const QString &text);
    QString getCurrentText(QTextCursor &txtCursor, bool fullTextOnNoSelection=false) const;
    QString getCurrentTextSurroundedByEmptyLines(QTextCursor &cursor) const;

    void setFoundTextPositions(const QList< QTextCursor > &foundTextPositions);
    void clearFoundTextPositions();

    void showLinePosition(int line, int linePos);

    void pulsate(const QTextCursor &cursor, int duration = 300);

    void setErrorPosition(const QTextCursor &cursor);
    void setErrorPositions(const QList< QTextCursor > &errorPositions);
    void addErrorPosition(const QTextCursor &cursor);

    void setMarkedLine(int line);

    static QFont currentFont;

public slots:
    void commentBlocks();
    void goToLine();
    void increaseFontSize();
    void decreaseFontSize();
    void resetFontSize();
    void toUpperCase();
    void toLowerCase();
    void makeDuplicate();
    void moveUp();
    void moveDown();
    void moveSelectedText(bool up);

    void customCut();
    void customCopy();

    void clearErrorPositions();
signals:
    void escapeKeyPressed();
    void gotFocus();
    void lostFocus();

protected:
    void resizeEvent(QResizeEvent *event);
    void keyReleaseEvent ( QKeyEvent * event );
    void keyPressEvent ( QKeyEvent * event );
    void focusInEvent ( QFocusEvent * event);
    void focusOutEvent ( QFocusEvent * event);
    void contextMenuEvent ( QContextMenuEvent * event );
    bool event ( QEvent * e );

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);
    void setUndoAvailable(bool available);
    void setRedoAvailable(bool available);
    void removePulsatePositions();
    void updateNavBar();

private:
    QWidget *lineNumberArea;
    LineNavigationBar *lineNavBar;

    QList< QTextCursor > foundTextPositions;
    QList< QTextCursor > pulsatePositions;
    QList< QTextCursor > errorPositions;

    QString strTab;

    int markedLineIx;

    void autoIndentNewBlock();
    void indentSelection();
    void unindentSelection();
    void handleHomeKey(bool keepSelection);
    int getFirstNonSpaceCharacterIndex(QTextCursor &cur, bool *onlySpaces=0);
    bool moveToFirstNonSpaceCharacter(QTextCursor &cur, QTextCursor::MoveMode moveMode=QTextCursor::MoveAnchor);
    void removeFromBeginning(QTextCursor &cur, int characterCount);
    void changeCase(bool toUpper);
    CursorPositionInfo getStartStopPositions(const QTextCursor cur);
    void moveSelectionUp();
    void moveSelectionDown();
    void ensureHasSelection();

    void drawLineNavBarHighlights(QPainter &painter);
    void drawLineNavBarHighlight(QPainter &painter, int blockNumber, const QColor &color,
                                 qreal docHeight, qreal blockHeight, bool hasScrollbars,
                                 bool fullLength = true);
    QList<int> getHighlightedBlockNumbers() const;

    bool lineMarkerUsed;
    bool lineMarkerVisible() const;
    int lineMarkerAreaWidth() const;
    int lineMarkerAreaOffset() const;

};

#endif // CODEEDITOR_H
