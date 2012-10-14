#ifndef TABLESPACELISTCOMBOBOX_H
#define TABLESPACELISTCOMBOBOX_H

#include "comboboxwithbutton.h"
#include "connectivity/queryresult.h"

class IQueryScheduler;

class TablespaceListComboBox : public ComboBoxWithButton
{
    Q_OBJECT
public:
    explicit TablespaceListComboBox(IQueryScheduler *queryScheduler=0,
                                    QWidget *parent = 0);

    void setQueryScheduler(IQueryScheduler *queryScheduler);

private:
    IQueryScheduler *queryScheduler;

private slots:
    void detailsButtonClicked();

};

#endif // TABLESPACELISTCOMBOBOX_H
