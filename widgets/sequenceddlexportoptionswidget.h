#ifndef SEQUENCEDDLEXPORTOPTIONSWIDGET_H
#define SEQUENCEDDLEXPORTOPTIONSWIDGET_H

#include <QWidget>
#include "beans/sequencecreateddloptions.h"

class QCheckBox;

class SequenceDdlExportOptionsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SequenceDdlExportOptionsWidget(QWidget *parent = 0);

    SequenceCreateDdlOptions getOptions() const;
    
private:
    QCheckBox *setInitialValueCheckBox;
    
};

#endif // SEQUENCEDDLEXPORTOPTIONSWIDGET_H
