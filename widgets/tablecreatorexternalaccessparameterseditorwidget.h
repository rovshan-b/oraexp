#ifndef TABLECREATOREXTERNALACCESSPARAMETERSEDITORWIDGET_H
#define TABLECREATOREXTERNALACCESSPARAMETERSEDITORWIDGET_H

#include <QWidget>

class CodeEditor;

class TableCreatorExternalAccessParametersEditorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TableCreatorExternalAccessParametersEditorWidget(QWidget *parent = 0);

    QString getAccessParams() const;

public slots:
    void setAccessParams(const QString &accessParams);

signals:
    void ddlChanged();

private:
    CodeEditor *editor;

};

#endif // TABLECREATOREXTERNALACCESSPARAMETERSEDITORWIDGET_H
