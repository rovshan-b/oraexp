#ifndef FILESELECTORWIDGET_H
#define FILESELECTORWIDGET_H

#include "widgets/lineeditwithbutton.h"

class FileSelectorWidget : public LineEditWithButton
{
    Q_OBJECT
public:
    enum FileMode
    {
        Save,
        Open
    };

    explicit FileSelectorWidget(FileMode mode, QWidget *parent = 0);

    void setDefaultSuffix(const QString &defaultSuffix);

    QString fileName() const;
    
private slots:
    void selectFilename();

private:
    FileMode mode;

    QString defaultSuffix;

    void correctFileSuffix();
    
};

#endif // FILESELECTORWIDGET_H
