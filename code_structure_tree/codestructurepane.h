#ifndef CODESTRUCTUREPANE_H
#define CODESTRUCTUREPANE_H

#include <QWidget>
#include <QHash>
#include <QModelIndex>

class QStackedWidget;
class CodeStructurePage;
class MultiEditorWidget;
class CodeStructureModel;

class CodeStructurePane : public QWidget
{
    Q_OBJECT
public:
    enum Pane
    {
        LabelPane,
        PagesPane
    };

    explicit CodeStructurePane(QWidget *parent = 0);
    
    virtual QSize sizeHint () const;

    void registerWidget(MultiEditorWidget *editor);
    void unregisterWidget(MultiEditorWidget *editor);

    void setCurrentEditor(MultiEditorWidget *editor);

    void setNewModel(MultiEditorWidget *editor, CodeStructureModel *newModel, const QModelIndex &index);

    void setActive(bool active);


    void showPane(Pane pane);

private:
    QStackedWidget *mainTab;
    QStackedWidget *pagesTab;

    QHash<MultiEditorWidget*, CodeStructurePage*> editors;

    bool isActive;

    void createLabelPane();
};

#endif // CODESTRUCTUREPANE_H
