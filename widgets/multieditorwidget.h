#ifndef MULTIEDITORWIDGET_H
#define MULTIEDITORWIDGET_H

#include <QWidget>
#include <QTime>

#include "codeeditorandsearchpanewidget.h"

class QSplitter;
class QActionGroup;
class QToolBar;
class QLabel;
class CodeCollapsePosition;

class MultiEditorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MultiEditorWidget(bool enableCodeCollapsing=false, QWidget *parent = 0);
    virtual ~MultiEditorWidget();
    
    CodeEditorAndSearchPaneWidget *getCurrentEditor() const;
    void addSplittingActions(QWidget *w);
    QLabel *createInfoLabel();

    void setReadOnly(bool readOnly);

    void setInfoLabelTextFormat(const QString &format);

    QList<CodeEditorAndSearchPaneWidget*> getEditors() const;
signals:
    void escapeKeyPressed();

protected:
    void timerEvent(QTimerEvent *event);

private slots:
    void editorCountActionSelected(bool checked);
    void editorOrientationActionSelected(QAction *action);
    void codeEditorFocusEvent(QWidget *object, bool focusIn);

    void cursorPositionChanged();

    void documentChanged();

private:
    void createUi();
    void setEditorCount(int count);
    int visibleEditorCount() const;
    QWidget *createEditor();

    QList<CodeEditorAndSearchPaneWidget*> editors;
    CodeEditorAndSearchPaneWidget *currentEditor;
    QSplitter *editorSplitter;
    QActionGroup *splitDirectionGroup;

    QLabel *infoLabel;
    QString infoLabelTextFormat;

    bool enableCodeCollapsing;

    int timerId;
    QTime lastChangeTime;

    QList<CodeCollapsePosition*> collapsePositions;
    void recalculateCollapsePositions();
    
};

#endif // MULTIEDITORWIDGET_H
