#include "codegeneratordialog.h"
#include "connection_page/dbuimanager.h"
#include "connection_page/worksheet/worksheet.h"
#include <QtGui>

CodeGeneratorDialog::CodeGeneratorDialog(DbUiManager *uiManager, QWidget *parent) :
    ConnectionPageWindow(uiManager, parent)
{
}

void CodeGeneratorDialog::acceptButtonPressed(QAbstractButton *button)
{
    if(button == buttonBox->button(QDialogButtonBox::Cancel)){
        return;
    }

    destination = (button==copyToClipboardButton) ? CDClipboard : CDWorksheet;

    if(isAsync()){
        setInProgress(true);
        generateCode();
    }else{
        QString code = generateCode();
        if(code.isEmpty()){
            QMessageBox::information(this, tr("No column selected"),
                                     tr("Please, select at least one column to generate DML."));
            return;
        }

        codeReady(code);
    }
}

void CodeGeneratorDialog::createButtonBox()
{
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel, Qt::Horizontal, this);
    copyToClipboardButton = new QPushButton(tr("Copy to clipboard"));
    buttonBox->addButton(copyToClipboardButton, QDialogButtonBox::AcceptRole);
    copyToClipboardButton->setAutoDefault(false);

    connect(buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(acceptButtonPressed(QAbstractButton*)));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

void CodeGeneratorDialog::codeReady(const QString &code)
{
    if(destination==CDClipboard){
        QApplication::clipboard()->setText(code);
    }else{
        uiManager->getWorksheet()->insertText(code);
    }

    accept();
}
