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
    explicit DbObjectComparerResultsTab(QWidget *parent = 0);

    virtual void createUi();
    virtual DataTable *createChangeListTable(DbUiManager *uiManager);

    void clearText();
    void addText(const QString &text);

    void showSearchWidget(){editor->showSearchPane();}
    void findNext(){editor->findNext();}
    void findPrevious(){editor->findPrevious();}

protected:
    QStandardItemModel *tableModel;
    CodeEditorAndSearchPaneWidget *editor;

    bool noText;

};

#endif // DBOBJECTCOMPARERRESULTSTAB_H
