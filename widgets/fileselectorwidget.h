#ifndef FILESELECTORWIDGET_H
#define FILESELECTORWIDGET_H

#include "widgets/lineeditwithbutton.h"

class FileSelectorWidget : public LineEditWithButton
{
    Q_OBJECT
public:
    explicit FileSelectorWidget(QWidget *parent = 0);

    void setDefaultSuffix(const QString &defaultSuffix);

    QString fileName() const;
    
private slots:
    void selectFilename();

private:
    void correctFileSuffix();

    QString defaultSuffix;
    
};

#endif // FILESELECTORWIDGET_H
