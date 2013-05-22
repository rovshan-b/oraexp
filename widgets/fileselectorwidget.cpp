#include "fileselectorwidget.h"
#include "util/dialoghelper.h"
#include <QtGui>

FileSelectorWidget::FileSelectorWidget(FileMode mode, QWidget *parent) :
    LineEditWithButton(parent),
    mode(mode)
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
    QString filename = (mode==Save)
                        ?
                       DialogHelper::showFileSaveDialog(this, tr("filename"), defaultSuffix)
                        :
                       DialogHelper::showFileOpenDialog(this, tr("Delimited file formats (*.csv *.tsv *.txt *.dsv);;All files (*.*)"));
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
