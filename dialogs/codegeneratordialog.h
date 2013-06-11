#ifndef CODEGENERATORDIALOG_H
#define CODEGENERATORDIALOG_H

#include "connection_page/connectionpagewindow.h"

class QDialogButtonBox;
class QAbstractButton;

class CodeGeneratorDialog : public ConnectionPageWindow
{
    Q_OBJECT
public:
    enum CodeDestination
    {
        CDClipboard,
        CDWorksheet
    };

    explicit CodeGeneratorDialog(DbUiManager *uiManager, QWidget *parent = 0);

protected slots:
    void acceptButtonPressed(QAbstractButton *button);
    
protected:
    QDialogButtonBox *buttonBox;
    QPushButton *copyToClipboardButton;

    void createButtonBox();

    CodeDestination destination;

    virtual bool isAsync() {return false;}
    virtual QString generateCode() = 0;
    void codeReady(const QString &code);
    
};

#endif // CODEGENERATORDIALOG_H
