#ifndef SEQUENCEDDLDIFFOPTIONSWIDGET_H
#define SEQUENCEDDLDIFFOPTIONSWIDGET_H

#include <QWidget>
#include "beans/sequencediffddloptions.h"

class QCheckBox;

class SequenceDdlDiffOptionsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SequenceDdlDiffOptionsWidget(QWidget *parent = 0);
    
    SequenceDiffDdlOptions getOptions() const;

private:
    QCheckBox *updateCurrValueCheckBox;
    
};

#endif // SEQUENCEDDLDIFFOPTIONSWIDGET_H
