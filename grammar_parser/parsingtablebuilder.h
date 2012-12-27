#ifndef PARSINGTABLEBUILDER_H
#define PARSINGTABLEBUILDER_H

class DFA;

#include <QStringList>
#include "parsingtablerow.h"
#include "ebnftoken.h"

class ParsingTableBuilder
{
public:
    explicit ParsingTableBuilder(DFA *dfa,
                                 const QStringList &keywords,
                                 const QList<EBNFToken> &tokens,
                                 int eofTokenId);
    ~ParsingTableBuilder();

private:
    DFA *dfa;
    QStringList keywords;
    QList<EBNFToken> tokens;
    int eofTokenId;

    QList<ParsingTableRow*> tableRows;


    void createParsingTable();
    void compactParsingTable();
    int getKeywordIx(const QString &keyword) const;

    void printoutParsingTable() const;
    void printoutTableRow(ParsingTableRow* row) const;

    void printoutForTargetParser();
    void printoutActionCode(const QString &varName, ParsingTableAction *action) const;
    void printoutRowCode(ParsingTableRow *row) const;

    QString getTokenName(int tokenId) const;

    //action,var name
    QHash<ParsingTableAction*, QString> actionVarNames;

};

#endif // PARSINGTABLEBUILDER_H
