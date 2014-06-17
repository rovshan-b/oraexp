#ifndef SELECTSTATEMENTFORMATTER_H
#define SELECTSTATEMENTFORMATTER_H

#include <QString>

class CodeScanner;

class SelectStatementFormatter
{
public:
    static QString format(CodeScanner *scanner, bool nested = false);

private:
    SelectStatementFormatter();

    enum SelectStatementSection
    {
        SelectList,
        IntoList,
        TableList,
        Where,
        GroupBy,
        Having,
        OrderBy
    };
};

#endif // SELECTSTATEMENTFORMATTER_H
