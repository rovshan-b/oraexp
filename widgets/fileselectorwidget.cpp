#include "fileselectorwidget.h"
#include "util/dialoghelper.h"
#include <QtGui>

FileSelectorWidget::FileSelectorWidget(QWidget *parent) :
    LineEditWithButton(parent)
{
    connect(this, SIGNAL(buttonClicked(LineEditWithButton*)), this, SLOT(selectFilename()));
}

void FileSelectorWidget::setDefaultSuffix(const QString &defaultSuffix)
{
    this->defaultSuffix = defaultSuffix;
    if(defaultSuffix.size()>0){
        correctFileSuffix();
    }
}

QString FileSelectorWidget::fileName() const
{
    return lineEdit()->text().trimmed();
}

void FileSelectorWidget::selectFilename()
{
    QString filename=DialogHelper::showFileSaveDialog(this, defaultSuffix);
    if(!filename.isEmpty()){
        lineEdit()->setText(filename);
    }
}

void FileSelectorWidget::correctFileSuffix()
{
    QString filename=lineEdit()->text().trimmed();

    if(filename.isEmpty()){
        return;
    }

    QFileInfo fileInfo(filename);

    QString currentSuffix = fileInfo.suffix();
    if(currentSuffix.isEmpty()){
        return;
    }

    if(currentSuffix.compare(defaultSuffix)==0){
        return;
    }

    filename.chop(currentSuffix.size());
    filename.append(defaultSuffix);

    lineEdit()->setText(filename);
}