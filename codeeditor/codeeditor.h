#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>

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

signals:
    void escapeKeyPressed();

protected:
    void resizeEvent(QResizeEvent *event);
    void keyReleaseEvent ( QKeyEvent * event );

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);

private:
    QWidget *lineNumberArea;

    QList< QTextCursor > foundTextPositions;
};

#endif // CODEEDITOR_H
