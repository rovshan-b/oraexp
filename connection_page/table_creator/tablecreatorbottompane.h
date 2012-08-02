#ifndef TABLECREATORBOTTOMPANE_H
#define TABLECREATORBOTTOMPANE_H

#include <QWidget>

class CodeEditor;
class QGroupBox;

class TableCreatorBottomPane : public QWidget
{
    Q_OBJECT
public:
    explicit TableCreatorBottomPane(QWidget *parent = 0);

    void setEditorText(const QString &text);
    void appendToEditor(const QString &text);

    QStringList getDdl() const;

private:
    CodeEditor *codeEditor;
    QGroupBox *ddlGroupBox;
};

#endif // TABLECREATORBOTTOMPANE_H
