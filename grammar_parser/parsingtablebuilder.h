#ifndef PARSINGTABLEBUILDER_H
#define PARSINGTABLEBUILDER_H

class DFA;

#include <QStringList>
#include "parsingtablerow.h"

class ParsingTableBuilder
{
public:
    explicit ParsingTableBuilder(DFA *dfa, const QStringList &keywords);
    ~ParsingTableBuilder();

private:
    DFA *dfa;
    QStringList keywords;

    QList<ParsingTableRow*> tableRows;


    void createParsingTable();
    void compactParsingTable();
    int getKeywordIx(const QString &keyword) const;

    void printoutParsingTable() const;
};

#endif // PARSINGTABLEBUILDER_H
