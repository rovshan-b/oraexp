#ifndef CODECREATOR_H
#define CODECREATOR_H

#include "../connectionpagetab.h"
#include "navtree/dbtreemodel.h"
#include "widgets/codeeditorandsearchpanewidget.h"

class DbObjectInfo;
class MetadataLoader;
class CodeEditorAndSearchPaneWidget;
class InfoPanel;
class QSplitter;
class QToolBar;
class QActionGroup;

class CodeCreator : public ConnectionPageTab
{
    Q_OBJECT
public:
    explicit CodeCreator(const QString &schemaName,
                         const QString &objectName,
                         DbTreeModel::DbTreeNodeType objectType,
                         DbUiManager *uiManager,
                         QWidget *parent = 0);

    virtual void createUi();
    virtual void setConnection(DbConnection *db);


    virtual bool canFind() const {return true;}
    virtual void showSearchWidget(){currentEditor->showSearchPane();}
    virtual void findNext() {currentEditor->findNext();}
    virtual void findPrevious() {currentEditor->findPrevious();}

protected:
    void keyReleaseEvent (QKeyEvent * event);

    
private slots:
    void objectInfoReady(DbObjectInfo *objectInfo, MetadataLoader *loader);
    void loadError(const QString &taskName, const OciException &ex, MetadataLoader *loader);
    void editorCountActionSelected(bool checked);
    void editorOrientationActionSelected(QAction *action);
    void codeEditorFocusEvent(QWidget *object, bool focusIn);

private:
    QWidget *createRightPane();
    QToolBar *createToolbar();
    void setEditorCount(int count);
    QWidget *createEditor();
    int visibleEditorCount() const;

    QString schemaName;
    QString objectName;
    DbTreeModel::DbTreeNodeType objectType;

    bool editMode;

    QSplitter *editorSplitter;
    CodeEditorAndSearchPaneWidget *currentEditor;
    InfoPanel *infoPanel;

    QActionGroup *splitDirectionGroup;

    QList<CodeEditorAndSearchPaneWidget*> editors;
    
};

#endif // CODECREATOR_H
