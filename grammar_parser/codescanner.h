#ifndef CODESCANNER_H
#define CODESCANNER_H

class QTextDocument;

class CodeScanner
{
public:
    CodeScanner(QTextDocument *doc);

private:
    QTextDocument *doc;
};

#endif // CODESCANNER_H
