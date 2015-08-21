#ifndef PLSQLPARSINGTABLESTATESUTIL_H_{id}
#define PLSQLPARSINGTABLESTATESUTIL_H_{id}

#include <QList>
#include "grammar_parser/parsingtablerow.h"

class PlSqlParsingTableStatesUtil{id}
{
public:
	static void initializeStates(QList<ParsingTableRow *> &rows);
};

#endif