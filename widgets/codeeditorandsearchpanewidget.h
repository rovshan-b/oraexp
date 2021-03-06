#ifndef CODEEDITORANDSEARCHPANEWIDGET_H
#define CODEEDITORANDSEARCHPANEWIDGET_H

#include <QWidget>
#include "codeeditor/codeeditor.h"
#include "searchpane.h"

class CodeEditor;
class IQueryScheduler;

class CodeEditorAndSearchPaneWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CodeEditorAndSearchPaneWidget(bool enableCodeCollapsing=false, QWidget *parent = 0);

    CodeEditor *editor() const {return this->codeEditor;}
    SearchPane *pane() const {return this->searchPane;}

    void setTextDocument(QTextDocument *document) const;
    QTextDocument *getTextDocument() const;

    void setQueryScheduler(IQueryScheduler *queryScheduler);

public slots:
    void showSearchPane();
    void findNext(){searchPane->findNext();}
    void findPrevious(){searchPane->findPrevious();}

    void setReadOnly(bool readOnly);
    void setInitialText(const QString &text);

    bool closeSearchPane();
signals:
    void escapeKeyPressed();
    void focusEvent(QWidget *object, bool focusIn);

protected:
    void keyReleaseEvent (QKeyEvent * event);
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void hideSearchPane();

private:
    CodeEditor *codeEditor;
    SearchPane *searchPane;

};

#endif // CODEEDITORANDSEARCHPANEWIDGET_H
