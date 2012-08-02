#ifndef LINEEDITWITHBUTTON_H
#define LINEEDITWITHBUTTON_H

#include <QWidget>

class LineEditWithAutocompleter;
class QPushButton;

class LineEditWithButton : public QWidget
{
    Q_OBJECT
public:
    explicit LineEditWithButton(QWidget *parent);

    LineEditWithAutocompleter *lineEdit() const;
    QPushButton *browseButton() const;

signals:
    void buttonClicked(LineEditWithButton *senderWidget);

private slots:
    void browseButtonClicked();

private:
    LineEditWithAutocompleter *editor;
    QPushButton *button;

};

#endif // LINEEDITWITHBUTTON_H
