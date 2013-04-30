#include "sourceddlexportoptionswidget.h"
#include "util/widgethelper.h"
#include <QtGui>

SourceDdlExportOptionsWidget::SourceDdlExportOptionsWidget(bool compareMode, QWidget *parent) :
    QWidget(parent), sourceIgnoreWhitespace(0)
{
    QGridLayout *sourceOptionsLayout=new QGridLayout();

    if(compareMode){
        sourceIgnoreWhitespace=WidgetHelper::createCheckBox(sourceOptionsLayout, 0, 0, tr("Ignore whitespace"), true);
    }
    sourceWrap=WidgetHelper::createCheckBox(sourceOptionsLayout, 1, 0, tr("Wrap"), false);
    sourceWrap->setEnabled(false);
    sourceWrap->setToolTip(tr("Will be enabled upon connecting to target database if version check is successfull"));

    sourceOptionsLayout->setContentsMargins(0,0,0,0);
    setLayout(sourceOptionsLayout);

    if(compareMode){
        connect(sourceWrap, SIGNAL(stateChanged(int)), this, SLOT(wrapCheckBoxChanged()));
    }
}

SourceCodeDdlOptions SourceDdlExportOptionsWidget::getOptions() const
{
    SourceCodeDdlOptions options;

    options.ignoreWhitespace=sourceIgnoreWhitespace ? sourceIgnoreWhitespace->isChecked() : false;
    options.wrap=sourceWrap->isChecked();

    return options;
}

void SourceDdlExportOptionsWidget::setCanWrap(bool canWrap, const QString &tooltip)
{
    sourceWrap->setEnabled(canWrap);
    sourceWrap->setToolTip(tooltip);
}

void SourceDdlExportOptionsWidget::wrapCheckBoxChanged()
{
    sourceIgnoreWhitespace->setEnabled(!sourceWrap->isChecked());
}
