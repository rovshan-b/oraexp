#ifndef CODESTRUCTUREPANE_H
#define CODESTRUCTUREPANE_H

#include <QWidget>
#include <QItemSelection>
#include "widgets/multieditorwidget.h"

class QStackedWidget;
class CodeStructureTreeView;
class PlSqlTreeBuilder;

class CodeStructurePane : public QWidget
{
    Q_OBJECT
public:
    enum Pane
    {
        TreePane,
        LabelPane
    };

    explicit CodeStructurePane(QWidget *parent = 0);
    
    virtual QSize sizeHint () const;

    void setCurrentWidget(MultiEditorWidget *editor);
    void setTreeBuilder(MultiEditorWidget *editor, PlSqlTreeBuilder *treeBuilder, int cursorPosition);
    PlSqlTreeBuilder *currentTreeBuilder() const;

    void setCursorPosition(MultiEditorWidget *editor, int position);

    void setActive(bool active);

    void unregisterWidget(MultiEditorWidget *editor);

    void showPane(Pane pane);

private slots:
    void treeCurrentRowChanged ( const QModelIndex & current, const QModelIndex & previous );
    void sortButtonClicked(bool sort);

private:
    QStackedWidget *tab;
    CodeStructureTreeView *treeView;

    MultiEditorWidget *currentEditor;
    PlSqlTreeBuilder *treeBuilder;

    QHash<MultiEditorWidget*,bool> sortedEditors;

    void setNewModel();

    bool isActive;

    QAction *sortAction;

    void createTreePane();
    void createLabelPane();
};

#endif // CODESTRUCTUREPANE_H
