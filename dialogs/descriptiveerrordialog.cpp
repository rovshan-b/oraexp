#include "descriptiveerrordialog.h"
#include "codeeditor/codeeditor.h"
#include "util/dialoghelper.h"
#include <QtGui>

DescriptiveErrorDialog::DescriptiveErrorDialog(const QString &title,
                                               const QString &errorMessage,
                                               const QString &code,
                                               unsigned int /*errorLine*/,
                                               QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(title);

    mainLayout=new QVBoxLayout();

    QHBoxLayout *errorMessageLayout=new QHBoxLayout();
    errorMessageLayout->setSpacing(errorMessageLayout->spacing()+10);

    QLabel *pixmapLabel=new QLabel();
    QStyle *style=qApp->style();
    QIcon errorIcon=style->standardIcon(QStyle::SP_MessageBoxCritical);
    int iconSize=style->pixelMetric(QStyle::PM_MessageBoxIconSize);
    pixmapLabel->setPixmap(errorIcon.pixmap(iconSize, iconSize));
    errorMessageLayout->addWidget(pixmapLabel);
    errorMessageLayout->setAlignment(pixmapLabel, Qt::AlignTop);

    QVBoxLayout *messageLayout=new QVBoxLayout();

    QLabel *errorTitleLabel=new QLabel("<b>"+title+"</b>");
    errorTitleLabel->setTextInteractionFlags(Qt::TextSelectableByMouse|Qt::TextSelectableByKeyboard);
    messageLayout->addWidget(errorTitleLabel);

    QLabel *errorMessageLabel=new QLabel(errorMessage);
    errorMessageLabel->setTextInteractionFlags(Qt::TextSelectableByMouse|Qt::TextSelectableByKeyboard);
    messageLayout->addWidget(errorMessageLabel);

    errorMessageLayout->addLayout(messageLayout);

    errorMessageLayout->setStretch(1, 1);

    mainLayout->addLayout(errorMessageLayout);


    QHBoxLayout *codeLayout=new QHBoxLayout();

    CodeEditor *editor=new CodeEditor();
    editor->setWordWrapMode(QTextOption::WordWrap);
    codeLayout->addWidget(editor);
    editor->setPlainText(code);

    mainLayout->addLayout(codeLayout);

    buttonBox=DialogHelper::createButtonBox(this, QDialogButtonBox::Ok);
    mainLayout->addWidget(buttonBox);

    //layout->setSpacing(15);

    setLayout(mainLayout);

    resize(500, 300);
}

void DescriptiveErrorDialog::showMessage(const QString &title,
                        const QString &errorMessage,
                        const QString &code,
                        unsigned int errorLine,
                        QWidget *parent)
{
    DescriptiveErrorDialog dialog(title, errorMessage, code, errorLine, parent);
    dialog.exec();
}

void DescriptiveErrorDialog::addWidget(QWidget *widget)
{
    mainLayout->insertWidget(mainLayout->indexOf(buttonBox), widget);
}
