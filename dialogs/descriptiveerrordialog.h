#ifndef DESCRIPTIVEERRORDIALOG_H
#define DESCRIPTIVEERRORDIALOG_H

#include <QDialog>

class QDialogButtonBox;
class QVBoxLayout;

class DescriptiveErrorDialog : public QDialog
{
    Q_OBJECT
public:
    explicit DescriptiveErrorDialog(const QString &title,
                                    const QString &errorMessage,
                                    const QString &code,
                                    unsigned int errorLine,
                                    QWidget *parent);

    static void showMessage(const QString &title,
                            const QString &errorMessage,
                            const QString &code,
                            unsigned int errorLine,
                            QWidget *parent);

    QDialogButtonBox *getButtonBox() const {return this->buttonBox;}
    void addWidget(QWidget *widget);

private:
    QVBoxLayout *mainLayout;
    QDialogButtonBox *buttonBox;
};

#endif // DESCRIPTIVEERRORDIALOG_H
