#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include "cursorpositioninfo.h"
#include "typedefs.h"

class LineNavigationBar;
class CodeCollapseArea;
class QCompleter;
class CodeCollapsePosition;

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    CodeEditor(bool enableCodeCollapsing = false, QWidget *parent = 0);
    virtual ~CodeEditor();

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth() const;
    //void lineNumberAreaWheelEvent(QWheelEvent * event);

    void codeCollapseAreaPaintEvent(QPaintEvent *event);
    int codeCollapseAreaWidth() const;
    void refreshCodeCollapseArea();

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

    void setCompleter(QCompleter *completer);
    //QCompleter *completer() const;

    void setCodeCollapsePositions(QList<CodeCollapsePosition*> *collapsePositions);

    virtual bool eventFilter ( QObject * watched, QEvent * event );

    static QList<CodeEditor*> openEditors;

    static QStringHash textShortcuts;
    static void loadTextShortcuts();

public slots:
    void commentBlocks();
    void goToLine();
    void increaseFontSize();
    void decreaseFontSize();
    void resetFontSize();
    void toUpperCase();
    void toLowerCase();
    void makeDuplicate();
    void removeEmptyLines();
    void moveUp();
    void moveDown();
    void moveSelectedText(bool up);
    void selectCurrentBlock();

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

    void insertCompletion(const QString &completion);

private:
    QWidget *lineNumberArea;
    CodeCollapseArea *codeCollapseArea;
    LineNavigationBar *lineNavBar;

    QList< QTextCursor > foundTextPositions;
    QList< QTextCursor > pulsatePositions;
    QList< QTextCursor > errorPositions;
    QTextCursor collapsibleRegionPositions;

    QString strTab;

    int markedLineIx;

    void autoIndentNewBlock();
    void indentSelection();
    void unindentSelection();
    void insertTab();
    bool insertTextShortcut(QTextCursor &cur);
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

    void highlightMatchingBraces(QList<QTextEdit::ExtraSelection> &extraSelections);
    QChar checkForBrace(int &pos);
    int findMatchingBrace(const QChar &brace, const QChar &matching, int fromPos, int toPos);

    QString textUnderCursor() const;

    QCompleter *completer;
    void completerHandleKeyPress(QKeyEvent * event);

    QList<CodeCollapsePosition*> *collapsePositions;
    CodeCollapsePosition* findCollapsePosition(int blockNumber);
    void highlightCollapsibleRegion(int blockNumber);
    void selectBlocks(QTextCursor &cur, const QTextBlock &startBlock, const QTextBlock &endBlock);

    void applyCurrentFontToAllEditors();
    void saveFontSettings();
};

#endif // CODEEDITOR_H
