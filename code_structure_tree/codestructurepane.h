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
    void setTreeBuilder(MultiEditorWidget *editor, PlSqlTreeBuilder *treeBuilder);
    PlSqlTreeBuilder *currentTreeBuilder() const;

private:
    CodeStructureTreeView *treeView;

    MultiEditorWidget *currentEditor;
    PlSqlTreeBuilder *treeBuilder;
};

#endif // CODESTRUCTUREPANE_H
