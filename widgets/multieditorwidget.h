#ifndef MULTIEDITORWIDGET_H
#define MULTIEDITORWIDGET_H

#include <QWidget>
#include <QTime>
#include <QTimer>
#include "codeeditorandsearchpanewidget.h"
#include "util/codereparser.h"

class QSplitter;
class QActionGroup;
class QToolBar;
class QLabel;
class CodeCollapsePosition;
class TokenInfo;
class IQueryScheduler;

class MultiEditorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MultiEditorWidget(bool enableCodeCollapsing=false, QWidget *parent = 0);
    virtual ~MultiEditorWidget();

    void setQueryScheduler(IQueryScheduler *queryScheduler);
    
    CodeEditorAndSearchPaneWidget *getCurrentEditor() const;
    CodeEditor *currentTextEditor() const {return currentEditor->editor();}
    QList<QAction *> addSplittingActions(QWidget *w);
    QLabel *createInfoLabel();

    void setInitialText(const QString &text);
    void setReadOnly(bool readOnly);

    void setInfoLabelTextFormat(const QString &format);

    QList<CodeEditorAndSearchPaneWidget*> getEditors() const;
signals:
    void escapeKeyPressed();
    void needsCompletionList();

private slots:
    void editorCountActionSelected(bool checked);
    void editorOrientationActionSelected(QAction *action);
    void codeEditorFocusEvent(QWidget *object, bool focusIn);

    void cursorPositionChanged();

    void documentContentsChanged(int position, int charsRemoved, int charsAdded);

    void onReparseTimer();

    void parsingCompleted(int requestId,
                          bool success,
                          PlSqlTreeBuilder *treeBulder,
                          int elapsedTime);

    void updateEditors(CodeEditor *except);
private:
    void createUi();
    void setEditorCount(int count);
    int visibleEditorCount() const;
    CodeEditorAndSearchPaneWidget *createEditor();

    QList<CodeEditorAndSearchPaneWidget*> editors;
    CodeEditorAndSearchPaneWidget *currentEditor;
    QSplitter *editorSplitter;
    QActionGroup *splitDirectionGroup;

    QLabel *infoLabel;
    QString infoLabelTextFormat;

    IQueryScheduler *queryScheduler;

    bool enableCodeCollapsing;

    bool isId(TokenInfo *token) const;
    bool isKeyword(TokenInfo *token) const;
    void applyCaseFoldingRules(int position, int charsRemoved, int charsAdded);
    void applyCaseFolding(CodeEditor::CaseFoldingType foldingType, int textPosition, QString &text, bool &changed);
    int lastEditedWordPosition;
    QString lastEditedWord;

    QTime lastChangeTime;
    QTime lastParseTime;
    QTimer reparseTimer;
    CodeReparser codeReparser;

    void reparse();
};

#endif // MULTIEDITORWIDGET_H
