#ifndef FILEENCODINGWIDGET_H
#define FILEENCODINGWIDGET_H

#include "comboboxandcheckboxwidget.h"

class FileEncodingWidget : public ComboBoxAndCheckBoxWidget
{
    Q_OBJECT
public:
    explicit FileEncodingWidget(QWidget *parent = 0);

    QString encoding() const;
    bool bom() const;
    
private slots:
    void encodingChanged();

private:
    bool enableBom() const;
    
};

#endif // FILEENCODINGWIDGET_H
