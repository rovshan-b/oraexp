#ifndef PLSQLPARSEHELPER_H
#define PLSQLPARSEHELPER_H

#include <QStringList>
#include <QTextCursor>
#include "beans/bindparaminfo.h"

class CodeCollapsePosition;
class PlSqlTreeBuilder;
class TextReaderBase;

class PlSqlParseHelper
{
public:
    static QStringList getBindParams(const QString &query, QList<BindParamInfo::BindParamType> *suggestedParamTypes=0);

    static QString getFirstLexeme(const QString &query);
    static bool isDml(const QString &query);

    static bool isDmlStartLexeme(const QString &lexeme);
    static bool isAnonymousBlockStartLexeme(const QString &lexeme);

    static void getNextQueryPos(const QString &query, int startFromPos, int *queryStartPos, int *queryEndPos);

    static void findObjectName(const QString &query, QString *schema, QString *name, const QString &defaultSchema);

    static QList<CodeCollapsePosition*> findCodeCollapsePositions(const QTextCursor &cur);

    static PlSqlTreeBuilder *createParseTree(TextReaderBase *reader, bool *success = 0);

private:
    static QString cleanIdentifier(const QString &identifier);

    PlSqlParseHelper();

};

#endif // PLSQLPARSEHELPER_H
