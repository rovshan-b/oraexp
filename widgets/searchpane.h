#ifndef SEARCHPANE_H
#define SEARCHPANE_H

#include <QWidget>
#include <QTextCursor>

class QLineEdit;
class CodeEditor;
class QToolButton;

class SearchPane : public QWidget
{
    Q_OBJECT
public:
    explicit SearchPane(CodeEditor *editor, QWidget *parent = 0);

    void activateFindTextBox();

    bool eventFilter(QObject *obj, QEvent *event);

public slots:
    void searchTextChanged(bool setSelection=true);
    QTextCursor findNextOrPrevious(bool next, const QTextCursor &baseCursor);
    bool findNext();
    bool findPrevious();

    void replace(bool highlightNext);
    void replaceOne();
    void replaceAndFind();
    void replaceAll();

    void closePane();

    QLineEdit *getFindTextBox() const {return this->findTextBox;}
    QLineEdit *getReplaceTextBox() const {return this->replaceTextBox;}

private:
    QLineEdit *findTextBox;
    QLineEdit *replaceTextBox;

    QToolButton *findNextButton;
    QToolButton *findPreviousButton;

    QToolButton *replaceOneButton;
    QToolButton *replaceAndFindButton;
    QToolButton *replaceAllButton;

    CodeEditor *editor;

    QLayout *createButtons();

};

#endif // SEARCHPANE_H
