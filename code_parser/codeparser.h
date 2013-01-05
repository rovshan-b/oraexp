#ifndef CODEPARSER_H
#define CODEPARSER_H

#include "codescanner.h"
#include "parsingtable.h"
#include <QStack>
#include <QPair>

class ParsingTable;
class ParsingTableRow;
class IReduceListener;

class CodeParser
{
public:
    CodeParser(CodeScanner *scanner);
    virtual ~CodeParser();

    virtual ParsingTable *getParsingTable()=0;

    bool parse();

    //first list is for terminals, second for rules
    QPair< QList<int>, QList<int> > getExpectedTokens() const;

    CodeScanner *getScanner() const {return this->scanner;}

    void setReduceListener(IReduceListener *listener) {this->reduceListener=listener;}

protected:
    CodeScanner *scanner;

    ParsingTableRow *errorRow;

private:
    QStack<int> stack;

    IReduceListener *reduceListener;
};

#endif // CODEPARSER_H
