#ifndef DESCRIPTIVEERRORDIALOG_H
#define DESCRIPTIVEERRORDIALOG_H

#include <QDialog>
#include "connectivity/ociexception.h"

class QDialogButtonBox;
class QVBoxLayout;

class DescriptiveErrorDialog : public QDialog
{
    Q_OBJECT
public:
    explicit DescriptiveErrorDialog(const QString &title,
                                    const OciException &exception,
                                    const QString &code,
                                    QWidget *parent,
                                    const QString &errorMessageOverride=QString());

    static void showMessage(const QString &title,
                            const OciException &exception,
                            const QString &code,
                            QWidget *parent, const QString &errorMessageOverride);

    QDialogButtonBox *getButtonBox() const {return this->buttonBox;}
    void addWidget(QWidget *widget);

private:
    QVBoxLayout *mainLayout;
    QDialogButtonBox *buttonBox;
};

#endif // DESCRIPTIVEERRORDIALOG_H
