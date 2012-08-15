#include "worksheet.h"
#include "worksheetresultpane.h"
#include "connectivity/dbconnection.h"
#include <iostream>
#include <QtGui>

using namespace std;

Worksheet::Worksheet(DbUiManager *uiManager, QWidget *parent) :
    ConnectionPageTab(uiManager, parent)
{
    QSplitter *splitter=new QSplitter(Qt::Vertical);
    queryPane = new WorksheetQueryPane();
    resultPane = new WorksheetResultPane();
    splitter->addWidget(queryPane);
    splitter->addWidget(resultPane);
    resultPane->setVisible(false);

    QHBoxLayout *layout=new QHBoxLayout();
    layout->addWidget(splitter);

    layout->setContentsMargins(0, 2, 0, 2);

    setLayout(layout);

    connect(queryPane, SIGNAL(queryDone(QueryResult)), this, SLOT(queryCompleted(QueryResult)));
}

Worksheet::~Worksheet()
{
    cout << "deleting worksheet"  << endl;
}

void Worksheet::setConnection(DbConnection *db)
{
    ConnectionPageTab::setConnection(db);

    queryPane->setConnection(db);
}

void Worksheet::queryCompleted(const QueryResult &result)
{
    if(!resultPane->isVisible()){
        resultPane->setVisible(true);
    }
    resultPane->displayQueryResults(this, result);
}

void Worksheet::setContents(const QString &contents)
{
    queryPane->setContents(contents);
}
