#include "codeeditordocument.h"
#include <QPlainTextDocumentLayout>
#include <QDebug>

CodeEditorDocument::CodeEditorDocument(QObject *parent) :
    QTextDocument(parent)
{
    QPlainTextDocumentLayout *documentLayout = new QPlainTextDocumentLayout(this);
    setDocumentLayout(documentLayout);

    connect(this, SIGNAL(contentsChange(int,int,int)), this, SLOT(handleContentChanged(int,int,int)));
    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(handleBlockCountChanged(int)));
}

void CodeEditorDocument::handleContentChanged(int position, int charsRemoved, int charsAdded)
{
    qDebug() << "CodeEditorDocument::handleContentChanged" << ", postition =" << position << ", removed =" << charsRemoved << ", added =" << charsAdded;
}

void CodeEditorDocument::handleBlockCountChanged(int newBlockCount)
{
    qDebug() << "new block count =" << newBlockCount;
}
