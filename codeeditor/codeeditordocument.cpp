#include "codeeditordocument.h"
#include <QPlainTextDocumentLayout>
#include <QSyntaxHighlighter>
#include <QDebug>

CodeEditorDocument::CodeEditorDocument(QObject *parent) :
    QTextDocument(parent), collapsedRangeCount(0)
{
    QPlainTextDocumentLayout *documentLayout = new QPlainTextDocumentLayout(this);
    setDocumentLayout(documentLayout);
}

void CodeEditorDocument::setHighlighter(QSyntaxHighlighter *highlighter)
{
    this->highlighter = highlighter;
}

QSyntaxHighlighter *CodeEditorDocument::getHighlighter() const
{
    return this->highlighter;
}

void CodeEditorDocument::forceSyntaxRehighlighting(const QTextBlock &block)
{
    this->highlighter->rehighlightBlock(block);
}

void CodeEditorDocument::incrementCollapsedRangeCount()
{
    ++collapsedRangeCount;
}

void CodeEditorDocument::decrementCollapsedRangeCount()
{
    --collapsedRangeCount;
    Q_ASSERT(collapsedRangeCount>=0);
}
