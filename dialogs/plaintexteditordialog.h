#ifndef PLAINTEXTEDITORDIALOG_H
#define PLAINTEXTEDITORDIALOG_H

#include <QDialog>

namespace Ui {
    class PlainTextEditorDialog;
}

class PlainTextEditorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlainTextEditorDialog(QWidget *parent = 0);
    ~PlainTextEditorDialog();

    QString getEditorText() const;
    void setEditorText(const QString &text);

private:
    Ui::PlainTextEditorDialog *ui;
};

#endif // PLAINTEXTEDITORDIALOG_H
