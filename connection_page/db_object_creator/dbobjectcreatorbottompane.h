#ifndef DBOBJECTCREATORBOTTOMPANE_H
#define DBOBJECTCREATORBOTTOMPANE_H

#include <QWidget>

class CodeEditor;
class QGroupBox;

class DbObjectCreatorBottomPane : public QWidget
{
    Q_OBJECT
public:
    explicit DbObjectCreatorBottomPane(QWidget *parent = 0);

    void setEditorText(const QString &text);
    void appendToEditor(const QString &text);

    QStringList getDdl() const;

private:
    CodeEditor *codeEditor;
    QGroupBox *ddlGroupBox;
    
};

#endif // DBOBJECTCREATORBOTTOMPANE_H
