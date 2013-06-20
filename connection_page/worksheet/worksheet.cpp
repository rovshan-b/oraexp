#include "worksheet.h"
#include "connectivity/dbconnection.h"
#include <iostream>
#include <QtGui>

Worksheet::Worksheet(DbUiManager *uiManager, QWidget *parent) :
    ConnectionPageTab(uiManager, parent)
{
    QVBoxLayout *mainLayout=new QVBoxLayout();

    worksheetWidget=new WorksheetWidget();
    mainLayout->addWidget(worksheetWidget);

    mainLayout->setContentsMargins(0,0,0,0);
    setLayout(mainLayout);

    connect(worksheetWidget, SIGNAL(autotraceTriggered(bool)), this, SLOT(autotraceTriggered(bool)));
    connect(worksheetWidget, SIGNAL(modificationChanged(bool)), this, SLOT(modificationChanged(bool)));
}

Worksheet::~Worksheet()
{
    qDebug("deleting worksheet");
}

void Worksheet::setConnection(DbConnection *db)
{
    ConnectionPageTab::setConnection(db);

    worksheetWidget->setQueryScheduler(this);

    setAutotraceEnabled(shouldCheckAutotraceAction());

    emitInitCompletedSignal();
}

void Worksheet::setContents(const QString &contents)
{
    worksheetWidget->setContents(contents);
}

QString Worksheet::getContents() const
{
    return worksheetWidget->getContents();
}

void Worksheet::insertText(const QString &text)
{
    worksheetWidget->insertText(text);
}

void Worksheet::focusAvailable()
{
    worksheetWidget->focusAvailable();
}

void Worksheet::setAutotraceEnabled(bool enabled)
{
    worksheetWidget->setAutotraceEnabled(enabled);
}

bool Worksheet::isAutotraceEnabled() const
{
    return worksheetWidget->isAutotraceEnabled();
}

bool Worksheet::isModified(int childIndex) const
{
    Q_UNUSED(childIndex);
    return worksheetWidget->isModified();
}

void Worksheet::setModified(bool modified, int childIndex)
{
    Q_UNUSED(childIndex);
    worksheetWidget->setModified(modified);
}

void Worksheet::saveToStream(QTextStream &out, int childIndex)
{
    Q_UNUSED(childIndex);
    out << worksheetWidget->getContents();
}

QString Worksheet::getCurrentFileName(int childIndex) const
{
    Q_UNUSED(childIndex);
    return this->currentFileName;
}

void Worksheet::setCurrentFileName(const QString &fileName, int childIndex)
{
    Q_UNUSED(childIndex);

    this->currentFileName = fileName;

    QFileInfo fileInfo(fileName);
    QString fileNamePart = fileInfo.fileName();
    setTitle(fileNamePart);
    setModifiedStatusToCaption(false);
}

void Worksheet::autotraceTriggered(bool checked)
{
    QList<ConnectionPageTab*> tabs = getPeerTabs();
    for(int i=0; i<tabs.size(); ++i){
        Worksheet *worksheet = static_cast<Worksheet*>(tabs.at(i));
        if(worksheet==this){
            continue;
        }
        worksheet->setAutotraceEnabled(checked);
    }
}

void Worksheet::modificationChanged(bool changed)
{
    setModifiedStatusToCaption(changed);
}

bool Worksheet::shouldCheckAutotraceAction() const
{
    QList<ConnectionPageTab*> tabs = getPeerTabs(1);
    if(tabs.size()==0){
        return false;
    }

    Worksheet *worksheet = qobject_cast<Worksheet*>(tabs.at(0));
    Q_ASSERT(worksheet);

    return worksheet->isAutotraceEnabled();
}
