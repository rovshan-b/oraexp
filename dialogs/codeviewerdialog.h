#ifndef CODEVIEWERDIALOG_H
#define CODEVIEWERDIALOG_H

#include <QDialog>

class CodeEditor;

class CodeViewerDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CodeViewerDialog(QWidget *parent = 0);
    
    void setCode(const QString &code);

private:
    CodeEditor *editor;
};

#endif // CODEVIEWERDIALOG_H
