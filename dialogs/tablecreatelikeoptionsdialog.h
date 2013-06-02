#ifndef TABLECREATELIKEOPTIONSDIALOG_H
#define TABLECREATELIKEOPTIONSDIALOG_H

#include <QDialog>
#include "beans/tablediffddloptions.h"

class ExistingTableOptionsWidget;
class IQueryScheduler;

class TableCreateLikeOptionsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TableCreateLikeOptionsDialog(IQueryScheduler *queryScheduler, QWidget *parent = 0);
    
    TableDiffDdlOptions getOptions() const;
private:
    ExistingTableOptionsWidget *optionsWidget;
    
};

#endif // TABLECREATELIKEOPTIONSDIALOG_H
