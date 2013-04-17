#ifndef SEQUANTIALEXECUTIONERRORDIALOG_H
#define SEQUANTIALEXECUTIONERRORDIALOG_H

#include "descriptiveerrordialog.h"

class QRadioButton;
class QButtonGroup;

class SequantialExecutionErrorDialog : public DescriptiveErrorDialog
{
    Q_OBJECT
public:
    enum ErrorAction
    {
        IgnoreAll,
        IgnoreCurrent,
        IgnoreCurrentCode,
        Abort
    };


    explicit SequantialExecutionErrorDialog(const QString &title,
                                            const OciException &exception,
                                            const QString &code,
                                            QWidget *parent);

    void setErrorAction(ErrorAction action);
    ErrorAction getErrorAction() const;
    
private:
    QRadioButton *ignoreAllRadio;
    QRadioButton *ignoreCurrentRadio;
    QRadioButton *ignoreCurrentCodeRadio;
    QRadioButton *abortRadio;

    QButtonGroup *radioGroup;
    
};

#endif // SEQUANTIALEXECUTIONERRORDIALOG_H
