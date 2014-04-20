#include "codeeditordocument.h"
#include <QPlainTextDocumentLayout>
#include <QDebug>

CodeEditorDocument::CodeEditorDocument(QObject *parent) :
    QTextDocument(parent)
{
    QPlainTextDocumentLayout *documentLayout = new QPlainTextDocumentLayout(this);
    setDocumentLayout(documentLayout);
}
