#ifndef SOURCEDDLEXPORTOPTIONSWIDGET_H
#define SOURCEDDLEXPORTOPTIONSWIDGET_H

#include <QWidget>
#include "beans/sourcecodeddloptions.h"

class QCheckBox;

class SourceDdlExportOptionsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SourceDdlExportOptionsWidget(bool compareMode,QWidget *parent = 0);

    SourceCodeDdlOptions getOptions() const;

    void setCanWrap(bool canWrap, const QString &tooltip);

private slots:
    void wrapCheckBoxChanged();
    
private:
    QCheckBox *sourceIgnoreWhitespace;
    QCheckBox *sourceWrap;
    
};

#endif // SOURCEDDLEXPORTOPTIONSWIDGET_H
