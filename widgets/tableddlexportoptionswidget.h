#ifndef TABLEDDLEXPORTOPTIONSWIDGET_H
#define TABLEDDLEXPORTOPTIONSWIDGET_H

#include <QWidget>
#include "beans/tablecreateddloptions.h"

class QCheckBox;

class TableDdlExportOptionsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TableDdlExportOptionsWidget(bool triggersOptionVisible = true, QWidget *parent = 0);

    TableCreateDdlOptions getOptions() const;

    void setFlashbackEnabled(bool enabled, const QString &tooltip);
    
private:
    QCheckBox *ntProperties;
    QCheckBox *ntFlashbackArchive;
    QCheckBox *ntIOTProperties;
    QCheckBox *ntLobProperties;
    QCheckBox *ntIndexes;
    QCheckBox *ntTriggers;
    QCheckBox *ntGrants;
    
};

#endif // TABLEDDLEXPORTOPTIONSWIDGET_H
