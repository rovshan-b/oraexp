#include "fileencodingwidget.h"
#include "util/widgethelper.h"

FileEncodingWidget::FileEncodingWidget(QWidget *parent) :
    ComboBoxAndCheckBoxWidget(tr("BOM") ,parent)
{
    WidgetHelper::fillAvailableTextCodecNames(this->comboBox());

    connect(this->comboBox(), SIGNAL(currentIndexChanged(QString)), this, SLOT(encodingChanged()));
}

QString FileEncodingWidget::encoding() const
{
    return comboBox()->currentText();
}

bool FileEncodingWidget::bom() const
{
    return checkBox()->isChecked() && enableBom();
}

void FileEncodingWidget::encodingChanged()
{
    checkBox()->setEnabled(enableBom());
}

bool FileEncodingWidget::enableBom() const
{
    return comboBox()->currentText().startsWith("UTF", Qt::CaseInsensitive);
}
