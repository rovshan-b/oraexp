#include "parsingtable.h"

ParsingTable::ParsingTable()
{
}

ParsingTable::~ParsingTable()
{
    qDeleteAll(rows);
}
