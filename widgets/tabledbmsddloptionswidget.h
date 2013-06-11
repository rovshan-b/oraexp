#ifndef TABLEDBMSDDLOPTIONSWIDGET_H
#define TABLEDBMSDDLOPTIONSWIDGET_H

#include <QWidget>
#include "beans/objectddloptions.h"

class QCheckBox;

class TableDbmsDdlOptionsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TableDbmsDdlOptionsWidget(QWidget *parent = 0);

    ObjectDdlOptions getOptions() const;
    void setOptions(const ObjectDdlOptions &options);

private slots:
    void enableControls();

private:
    QCheckBox *prettyPrintCheckBox;
    QCheckBox *sqlTerminatorsCheckBox;
    QCheckBox *segmentAttributesCheckBox;
    QCheckBox *storageCheckBox;
    QCheckBox *tablespaceCheckBox;
    QCheckBox *constraintsCheckBox;
    QCheckBox *constraintsAsAlterCheckBox;
    QCheckBox *refConstraintsCheckBox;
    QCheckBox *oidCheckBox;
    QCheckBox *sizeByteKeywordCheckBox;

    QString getWidgetValue(QCheckBox *checkBox) const;
    void setWidgetValue(QCheckBox *checkBox, const QString &value);


};

#endif // TABLEDBMSDDLOPTIONSWIDGET_H
