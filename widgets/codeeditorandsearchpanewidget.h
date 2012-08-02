#ifndef CODEEDITORANDSEARCHPANEWIDGET_H
#define CODEEDITORANDSEARCHPANEWIDGET_H

#include <QWidget>
#include "codeeditor/codeeditor.h"
#include "searchpane.h"

class CodeEditor;

class CodeEditorAndSearchPaneWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CodeEditorAndSearchPaneWidget(QWidget *parent = 0);

    CodeEditor *editor(){return this->codeEditor;}
    SearchPane *pane(){return this->searchPane;}

    void showSearchPane();
    void findNext(){searchPane->findNext();}
    void findPrevious(){searchPane->findPrevious();}

protected:
    void keyReleaseEvent (QKeyEvent * event);

private slots:
    void hideSearchPane();

private:
    CodeEditor *codeEditor;
    SearchPane *searchPane;

};

#endif // CODEEDITORANDSEARCHPANEWIDGET_H
