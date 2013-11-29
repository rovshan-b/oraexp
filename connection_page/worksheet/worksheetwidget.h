#ifndef WORKSHEETWIDGET_H
#define WORKSHEETWIDGET_H

#include <QWidget>

#include "connectivity/queryresult.h"
#include "../connectionpagetab.h"
#include "worksheetquerypane.h"

class WorksheetResultPane;
class QSplitter;
class DbUiManager;

class WorksheetWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WorksheetWidget(DbUiManager *uiManager, QWidget *parent = 0);
    
    void setQueryScheduler(IQueryScheduler *queryScheduler);

    void setContents(const QString &contents);
    QString getContents() const;

    void insertText(const QString &text);

    void focusAvailable();

    void setAutotraceEnabled(bool enabled);
    bool isAutotraceEnabled() const;

    bool isModified() const;
    void setModified(bool modified);

    QTextDocument *getTextDocument();

    static QByteArray splitterSizes;
signals:
    void autotraceTriggered(bool checked);
    void modificationChanged(bool changed);

public slots:
    void queryCompleted(const QueryResult &result);
    void splitterMoved();
    void handleQueryPaneMessage(const QString &msg);

    void hideResultPane();

private:
    QSplitter *splitter;
    WorksheetQueryPane *queryPane;
    WorksheetResultPane *resultPane;

    IQueryScheduler *queryScheduler;

    bool resultPaneShownBefore;

    void showResultPane();
    
};

#endif // WORKSHEETWIDGET_H
