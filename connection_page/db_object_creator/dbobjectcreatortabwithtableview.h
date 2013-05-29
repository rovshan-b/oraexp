#ifndef DBOBJECTCREATORTABWITHTABLEVIEW_H
#define DBOBJECTCREATORTABWITHTABLEVIEW_H

#include "dbobjectcreatortab.h"
#include "widgets/datatable.h"
#include "widgets/tabletoolbar.h"
#include "util/widgethelper.h"
#include "util/iconutil.h"
#include <QtGui>

class DataTable;
class QToolBar;
class QAction;

template <typename ParentWidgetType>
class DbObjectCreatorTabWithTableView : public DbObjectCreatorTab<ParentWidgetType>
{
public:
    explicit DbObjectCreatorTabWithTableView(ParentWidgetType *objectCreator, bool editMode, QWidget *parent = 0) :
        DbObjectCreatorTab<ParentWidgetType>(objectCreator, editMode, parent)
    {
        QVBoxLayout *layout=new QVBoxLayout();

        table=new DataTable();
        toolbar=new TableToolbar(table);

        advancedOptionsAction=new QAction(IconUtil::getIcon("advanced_options"),
                                          QObject::tr("Show/Hide advanced options"), this);
        advancedOptionsAction->setCheckable(true);

        toolbar->addSeparator();
        toolbar->addAction(advancedOptionsAction);

        layout->addWidget(toolbar);
        layout->addWidget(table);

        layout->setContentsMargins(2, 2, 2, 0);

        //connect(advancedOptionsAction, SIGNAL(toggled(bool)), this, SLOT(showAdvancedOptions(bool)));

        this->setLayout(layout);
    }

    virtual void removeIncorrectRows()
    {
        WidgetHelper::removeIncorrectRows(table);
    }

    DataTable *getTable() const {return this->table;}

protected:
    DataTable *table;
    QToolBar *toolbar;
    QAction* advancedOptionsAction;
};

#endif // DBOBJECTCREATORTABWITHTABLEVIEW_H
