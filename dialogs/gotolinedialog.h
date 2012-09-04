#ifndef GOTOLINEDIALOG_H
#define GOTOLINEDIALOG_H

#include <QDialog>

class QSpinBox;

class GoToLineDialog : public QDialog
{
    Q_OBJECT
public:
    explicit GoToLineDialog(int currentLine, int maxLineCount, QWidget *parent);

    int getLine() const;
    
private:
    QSpinBox *lineNumberSelector;
    
};

#endif // GOTOLINEDIALOG_H
