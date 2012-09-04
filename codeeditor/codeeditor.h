#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include "cursorpositioninfo.h"

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    CodeEditor(QWidget *parent = 0);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

    //use only when sure that there's not lots of text in editor
    QStringList getSemicolonSeparated() const;

    void addText(const QString &text);
    QString getCurrentText() const;

    void setFoundTextPositions(const QList< QTextCursor > &foundTextPositions);
    void clearFoundTextPositions();

    void showLinePosition(int line, int linePos);

public slots:
    void commentBlocks();
    void goToLine();
    void increaseFontSize();
    void decreaseFontSize();
    void toUpperCase();
    void toLowerCase();
    void moveUp();
    void moveDown();
    void moveSelection(bool up);

signals:
    void escapeKeyPressed();

protected:
    void resizeEvent(QResizeEvent *event);
    void keyReleaseEvent ( QKeyEvent * event );
    void keyPressEvent ( QKeyEvent * event );

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);

private:
    QWidget *lineNumberArea;

    QList< QTextCursor > foundTextPositions;

    QString strTab;

    void autoIndentNewBlock();
    void indentSelection();
    void unindentSelection();
    void handleHomeKey(bool keepSelection);
    int getFirstNonSpaceCharacterIndex(QTextCursor &cur, bool *onlySpaces=0);
    bool moveToFirstNonSpaceCharacter(QTextCursor &cur, QTextCursor::MoveMode moveMode=QTextCursor::MoveAnchor);
    void removeFromBeginning(QTextCursor &cur, int characterCount);
    void changeCase(bool toUpper);
    CursorPositionInfo getStartStopPositions(const QTextCursor cur);

};

#endif // CODEEDITOR_H
