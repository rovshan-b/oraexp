#ifndef CODESTRUCTUREPAGE_H
#define CODESTRUCTUREPAGE_H

#include <QWidget>
#include <QItemSelection>

class CodeStructureTreeView;
class MultiEditorWidget;
class CodeStructureModel;
class CodeStructureSortFilterModel;

class CodeStructurePage : public QWidget
{
    Q_OBJECT
public:
    explicit CodeStructurePage(MultiEditorWidget *editor, QWidget *parent = 0);

    void setCursorPosition(const QModelIndex &index);
    
    void setNewModel(CodeStructureModel *newModel);
private slots:
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void treeCurrentRowChanged ( const QModelIndex & current, const QModelIndex & previous );
    void sortButtonClicked(bool sort);

    void codeStructureModelAvailable(CodeStructureModel *model);
    void cursorPositionChanged(const QModelIndex &index);

private:
    CodeStructureTreeView *treeView;
    CodeStructureSortFilterModel *sortModel;
    MultiEditorWidget *editor;

    QAction *sortAction;

    void createTreePane();
    
};

#endif // CODESTRUCTUREPAGE_H
