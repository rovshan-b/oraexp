#ifndef LOBPARAMSDIALOG_H
#define LOBPARAMSDIALOG_H

#include <QDialog>
#include "beans/lobparams.h"

class IQueryScheduler;
class LobParamsWidget;

class LobParamsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit LobParamsDialog(IQueryScheduler *queryScheduler, QWidget *parent, bool isEditMode);

    LobParams getParams() const;
    void setParams(const LobParams &params, const LobParams &) const;

private:
    LobParamsWidget *lobParamsWidget;

private slots:
    void okButtonPressed();

};

#endif // LOBPARAMSDIALOG_H
