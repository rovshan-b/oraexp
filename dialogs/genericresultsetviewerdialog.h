#ifndef GENERICRESULTSETVIEWERDIALOG_H
#define GENERICRESULTSETVIEWERDIALOG_H

#include <QDialog>
#include <QModelIndex>

class DataTable;
class IQueryScheduler;
class Param;

class GenericResultsetViewerDialog : public QDialog
{
    Q_OBJECT
public:
    explicit GenericResultsetViewerDialog(IQueryScheduler *queryScheduler,
                                          const QString &query,
                                          const QList<Param*> &params,
                                          const QString &dbLinkName,
                                          QWidget *parent,
                                          const QPair<QString,QString> &iconColumn=QPair<QString,QString>());

    QString selectedText;

private slots:
    void rowActivated(const QModelIndex &index);

private:
    DataTable *table;

};

#endif // GENERICRESULTSETVIEWERDIALOG_H
