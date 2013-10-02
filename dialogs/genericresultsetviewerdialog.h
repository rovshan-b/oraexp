#ifndef GENERICRESULTSETVIEWERDIALOG_H
#define GENERICRESULTSETVIEWERDIALOG_H

#include <QDialog>
#include <QModelIndex>

class DataTable;
class IQueryScheduler;
class Param;
class CodeEditor;

class GenericResultsetViewerDialog : public QDialog
{
    Q_OBJECT
public:
    explicit GenericResultsetViewerDialog(IQueryScheduler *queryScheduler,
                                          const QString &query,
                                          const QList<Param*> &params,
                                          const QString &dbLinkName,
                                          QWidget *parent,
                                          const QPair<QString,QString> &iconColumn=QPair<QString,QString>(),
                                          bool displayEditor = false);

    QString selectedText;

    QString currentQuery() const;

private slots:
    void rowActivated(const QModelIndex &index);
    void executeQuery();

    void restoreQueryButtonText();

private:
    IQueryScheduler *queryScheduler;

    DataTable *table;

    CodeEditor *editor;
    QPushButton *runQueryButton;

};

#endif // GENERICRESULTSETVIEWERDIALOG_H
