#ifndef PLSQLPARSEHELPER_H
#define PLSQLPARSEHELPER_H

#include <QStringList>
#include <QTextCursor>
#include "beans/bindparaminfo.h"

class CodeCollapsePosition;
class PlSqlTreeBuilder;
class TextReaderBase;
class PlSqlScanner;

class PlSqlParseHelper
{
public:
    static QStringList getBindParams(const QString &query, QList<BindParamInfo::BindParamType> *suggestedParamTypes=0);

    static QString getFirstLexeme(const QString &query);
    static bool isDml(const QString &query);

    static bool isDmlStartLexeme(const QString &lexeme);
    static bool isAnonymousBlockStartLexeme(const QString &lexeme);

    static void getNextQueryPos(const QString &query, int startFromPos, int *queryStartPos, int *queryEndPos);

    static void findObjectName(PlSqlTreeBuilder *treeBuilder, QString *schema, QString *name, const QString &defaultSchema);

    //static void findObjectName(const QString &query, QString *schema, QString *name, const QString &defaultSchema);

    static PlSqlTreeBuilder *createParseTree(TextReaderBase *reader, bool *success = 0);

    static void findTableNameInSelectQuery(const QString &query, QString *schemaName, QString *tableName, QString *dblink, const QString &defaultSchemaName = "");

    static void readTableName(PlSqlScanner *scanner, QString *schemaName, QString *tableName, QString *dblink, const QString &defaultSchemaName);

    static QString readMultiPartNameAsOne(PlSqlScanner *scanner);

    static QStringList readMultiPartName(PlSqlScanner *scanner);

    static void prepareViewForEditing(QString &query);

    static void prepareTriggerForEditing(QString &query);

    static bool readToMatchingParentheses(PlSqlScanner *scanner);

    static int extractPlSqlErrorCode(const QString &errorMessage);

    //static QStringList tokenizeName(const QString &objectName);

    static QString cleanIdentifier(const QString &identifier);
private:

    PlSqlParseHelper();

};

#endif // PLSQLPARSEHELPER_H
