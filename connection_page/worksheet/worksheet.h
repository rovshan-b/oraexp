#ifndef WORKSHEET_H
#define WORKSHEET_H

#include <QWidget>
#include "connectivity/queryresult.h"
#include "../connectionpagetab.h"
#include "worksheetquerypane.h"

class WorksheetResultPane;

class Worksheet : public ConnectionPageTab
{
    Q_OBJECT
public:
    explicit Worksheet(DbUiManager *uiManager, QWidget *parent = 0);
    virtual ~Worksheet();

    virtual void setConnection(DbConnection *db);

    void setContents(const QString &contents);

    virtual bool disableWhileConnecting(){return false;}

    virtual bool canSave() const {return true;}
    virtual bool canPrint() const {return true;}
    virtual bool canFind() const {return true;}

    virtual void showSearchWidget(){queryPane->showSearchWidget();}
    virtual void findNext() {queryPane->findNext();}
    virtual void findPrevious() {queryPane->findPrevious();}

public slots:
    void queryCompleted(const QueryResult &result);

private:
    WorksheetQueryPane *queryPane;
    WorksheetResultPane *resultPane;

};

#endif // WORKSHEET_H
