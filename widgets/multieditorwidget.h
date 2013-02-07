#ifndef MULTIEDITORWIDGET_H
#define MULTIEDITORWIDGET_H

#include <QWidget>

#include "codeeditorandsearchpanewidget.h"

class QSplitter;
class QActionGroup;
class QToolBar;
class QLabel;

class MultiEditorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MultiEditorWidget(QWidget *parent = 0);
    
    CodeEditorAndSearchPaneWidget *getCurrentEditor() const;
    void addSplittingActions(QWidget *w);
    QLabel *createInfoLabel();

    void setReadOnly(bool readOnly);

    void setInfoLabelTextFormat(const QString &format);
signals:
    void escapeKeyPressed();

private slots:
    void editorCountActionSelected(bool checked);
    void editorOrientationActionSelected(QAction *action);
    void codeEditorFocusEvent(QWidget *object, bool focusIn);

    void cursorPositionChanged();

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
    
};

#endif // MULTIEDITORWIDGET_H
