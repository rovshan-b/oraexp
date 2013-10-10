#ifndef PLAINTEXTEDITORDIALOG_H
#define PLAINTEXTEDITORDIALOG_H

#include <QDialog>

class QPlainTextEdit;

class PlainTextEditorDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PlainTextEditorDialog(QWidget *parent, bool useCodeEditor = false);

    void setEditorText(const QString &text);
    QString getEditorText() const;
    
private:
    QPlainTextEdit *editor;
    
};

#endif // PLAINTEXTEDITORDIALOG_H
