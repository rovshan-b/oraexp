#ifndef DBOBJECTCOMPARERRESULTSTAB_H
#define DBOBJECTCOMPARERRESULTSTAB_H

#include "widgets/codeeditorandsearchpanewidget.h"
#include "interfaces/isearchable.h"

class QStandardItemModel;
class CodeEditorAndSearchPaneWidget;
class DataTable;
class DbUiManager;

class DbObjectComparerResultsTab : public QWidget, public ISearchable
{
    Q_OBJECT
public:
    explicit DbObjectComparerResultsTab(DbUiManager *uiManager, QWidget *parent = 0);

    virtual void createUi();
    virtual DataTable *createChangeListTable();

    void addText(const QString &text);

    void showSearchWidget(){editor->showSearchPane();}
    void findNext(){editor->findNext();}
    void findPrevious(){editor->findPrevious();}

    void beforeCompare();
    void afterCompare();

public slots:
    virtual void setCurrentSchema(const QString &currentSchemaName);

protected:
    DbUiManager *uiManager;
    DataTable *changeListTable;
    QStandardItemModel *tableModel;
    CodeEditorAndSearchPaneWidget *editor;

    bool noText;

};

#endif // DBOBJECTCOMPARERRESULTSTAB_H
