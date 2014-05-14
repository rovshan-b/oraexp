#ifndef CODEEDITORDOCUMENT_H
#define CODEEDITORDOCUMENT_H

#include <QTextDocument>

class QSyntaxHighlighter;

class CodeEditorDocument : public QTextDocument
{
    Q_OBJECT
public:
    explicit CodeEditorDocument(QObject *parent);

    void setHighlighter(QSyntaxHighlighter *highlighter);
    QSyntaxHighlighter *getHighlighter() const;

    void forceSyntaxRehighlighting(const QTextBlock &block);

    void incrementCollapsedRangeCount();
    void decrementCollapsedRangeCount();
    int getCollapsedRangeCount() const {return this->collapsedRangeCount;}

private:
    QSyntaxHighlighter *highlighter;

    int collapsedRangeCount;
    
};

#endif // CODEEDITORDOCUMENT_H
