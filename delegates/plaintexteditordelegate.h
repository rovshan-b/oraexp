#ifndef PLAINTEXTEDITORDELEGATE_H
#define PLAINTEXTEDITORDELEGATE_H

#include "lineeditwithbuttondelegate.h"

class PlainTextEditorDelegate : public LineEditWithButtonDelegate
{
    Q_OBJECT
public:
    explicit PlainTextEditorDelegate(const QString &dialogTitle, QObject *parent);

private slots:
    virtual void buttonClicked(LineEditWithButton *senderWidget);

private:
    QString dialogTitle;

};

#endif // PLAINTEXTEDITORDELEGATE_H
