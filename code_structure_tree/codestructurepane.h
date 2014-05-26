#ifndef CODESTRUCTUREPANE_H
#define CODESTRUCTUREPANE_H

#include <QWidget>
#include "widgets/multieditorwidget.h"

class CodeStructureTreeView;
class PlSqlTreeBuilder;

class CodeStructurePane : public QWidget
{
    Q_OBJECT
public:
    explicit CodeStructurePane(QWidget *parent = 0);
    
    virtual QSize sizeHint () const;

    void setCurrentWidget(MultiEditorWidget *editor);
    void setTreeBuilder(MultiEditorWidget *editor, PlSqlTreeBuilder *treeBuilder, int cursorPosition);
    PlSqlTreeBuilder *currentTreeBuilder() const;

    void setCursorPosition(MultiEditorWidget *editor, int position);

    void setActive(bool active);

    void unregisterWidget(MultiEditorWidget *editor);

private:
    CodeStructureTreeView *treeView;

    MultiEditorWidget *currentEditor;
    PlSqlTreeBuilder *treeBuilder;

    void setNewModel();

    bool isActive;
};

#endif // CODESTRUCTUREPANE_H
