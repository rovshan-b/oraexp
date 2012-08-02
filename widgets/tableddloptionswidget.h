#ifndef TABLEDDLOPTIONSWIDGET_H
#define TABLEDDLOPTIONSWIDGET_H

#include <QWidget>
#include "beans/objectddloptions.h"

class QComboBox;

class TableDdlOptionsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TableDdlOptionsWidget(QWidget *parent = 0);

    ObjectDdlOptions getOptions() const;
    void setOptions(const ObjectDdlOptions &options);

private slots:
    void enableControls();

private:
    QComboBox *prettyPrintCombo;
    QComboBox *sqlTerminatorsCombo;
    QComboBox *segmentAttributesCombo;
    QComboBox *storageCombo;
    QComboBox *tablespaceCombo;
    QComboBox *constraintsCombo;
    QComboBox *constraintsAsAlterCombo;
    QComboBox *refConstraintsCombo;
    QComboBox *oidCombo;
    QComboBox *sizeByteKeywordCombo;

    void addStandardItems(QComboBox *comboBox);

    QString getWidgetValue(QComboBox *comboBox) const;
    void setWidgetValue(QComboBox *comboBox, const QString &value);


};

#endif // TABLEDDLOPTIONSWIDGET_H
