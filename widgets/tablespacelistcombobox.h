#ifndef TABLESPACELISTCOMBOBOX_H
#define TABLESPACELISTCOMBOBOX_H

#include "comboboxwithbutton.h"
#include "connectivity/queryresult.h"

class IQueryScheduler;

class TablespaceListComboBox : public ComboBoxWithButton
{
    Q_OBJECT
public:
    explicit TablespaceListComboBox(IQueryScheduler *queryScheduler,
                                    QWidget *parent = 0);


private:
    IQueryScheduler *queryScheduler;

private slots:
    void detailsButtonClicked();

};

#endif // TABLESPACELISTCOMBOBOX_H
