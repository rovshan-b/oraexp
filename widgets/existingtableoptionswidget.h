#ifndef EXISTINGTABLEOPTIONSWIDGET_H
#define EXISTINGTABLEOPTIONSWIDGET_H

#include <QWidget>
#include "beans/tablediffddloptions.h"

class QCheckBox;

class ExistingTableOptionsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ExistingTableOptionsWidget(bool triggersOptionVisible = true, QWidget *parent = 0);

    TableDiffDdlOptions getOptions() const;

    void setFlashbackEnabled(bool enabled, const QString &tooltip);

private slots:
    void optionsChanged(int);
    
private:
    QCheckBox *etComments;
    QCheckBox *etProperties;
    QCheckBox *etFlashbackArchive;
    QCheckBox *etStorage;
    QCheckBox *etIOTProperties;
    QCheckBox *etLOBProperties;
    QCheckBox *etColumns;
    QCheckBox *etConstraints;
    QCheckBox *etIndexes;
    QCheckBox *etPartitions;
    QCheckBox *etExternalProperties;
    QCheckBox *etTriggers;
    QCheckBox *etGrants;
    
};

#endif // EXISTINGTABLEOPTIONSWIDGET_H
