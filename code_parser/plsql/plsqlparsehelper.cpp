#include "plsqlparsehelper.h"
#include "plsqlscanner.h"
#include "../stringreader.h"
#include "../textcursorreader.h"
#include "../codeparser.h"
#include "../codeparserfactory.h"
#include "plsqltokens.h"
#include "plsqlparsingtable.h"
#include "plsqltreebuilder.h"
#include "beans/codecollapseposition.h"
#include "beans/parsetreenode.h"
#include "util/dbutil.h"
#include <QDebug>

QStringList PlSqlParseHelper::getBindParams(const QString &query, QList<BindParamInfo::BindParamType> *suggestedParamTypes)
{
    QStringList results;

    QScopedPointer<PlSqlScanner> scanner(new PlSqlScanner(new StringReader(query)));


    int token = PLS_E_O_F;
    bool waitingForName=false;
    bool metReturningIntoParam=false;
    bool isDmlStatement = isDml(query);
    QStringList lastLexemes;
    lastLexemes.reserve(3);

    do{
        token = scanner->getNextToken();
        QString lastLexeme = scanner->getTokenLexeme();

        if(lastLexemes.isEmpty()){ //just got first lexeme
            if(!PlSqlParseHelper::isDmlStartLexeme(lastLexeme) &&
                    !PlSqlParseHelper::isAnonymousBlockStartLexeme(lastLexeme)){ //prompting for bind variables on for dmls and anonymous blocks
                return results;
            }
        }

        lastLexemes.append(lastLexeme);
        if(lastLexemes.size()>4){
            lastLexemes.removeAt(0);
        }

        if(token==PLS_COLON){ //waiting for bind variable name
            waitingForName=true;
        }else if(waitingForName){
            waitingForName=false;

            if(token!=PLS_ID && token!=PLS_DOUBLEQUOTED_STRING && !(token<NON_LITERAL_START_IX)){
                continue;
            }

            //must keep single entry for non double quoted strings regardless of case
            //and must keep single entry for double quoted strings taking into account case
            QString paramName = scanner->getTokenLexeme();
            if(!results.contains(paramName,
                                 token==PLS_DOUBLEQUOTED_STRING ?
                                        Qt::CaseSensitive :
                                        Qt::CaseInsensitive)){
                results.append(token==PLS_DOUBLEQUOTED_STRING ? paramName : paramName.toUpper());
            }else{
                continue;
            }

            if(suggestedParamTypes!=0){
                if(lastLexemes.size()>=3 && lastLexemes.at(lastLexemes.size()-3).compare("OPEN", Qt::CaseInsensitive)==0){
                    suggestedParamTypes->append(BindParamInfo::Cursor);
                }else if(metReturningIntoParam || (isDmlStatement &&
                                                   lastLexemes.size()>=3 &&
                                                   lastLexemes.at(lastLexemes.size()-3).compare("INTO", Qt::CaseInsensitive)==0)){
                    suggestedParamTypes->append(BindParamInfo::ReturningInto);
                    metReturningIntoParam = true;
                }else {
                    QStringList nameParts=paramName.split('_');
                    if(nameParts.contains("DATE", Qt::CaseInsensitive) ||
                            nameParts.contains("TIME", Qt::CaseInsensitive) ||
                            nameParts.contains("DATETIME", Qt::CaseInsensitive)){
                        suggestedParamTypes->append(BindParamInfo::Date);
                    }else{
                        suggestedParamTypes->append(BindParamInfo::StringOrNumber);
                    }
                }
            }
        }

    }while(token!=PLS_E_O_F && token!=PLS_ERROR);

    if(token==PLS_ERROR){
        qDebug("encountered PLS_ERROR, cleaning up results");
        results.clear();
    }

    qDebug() << results;

    return results;
}

QString PlSqlParseHelper::getFirstLexeme(const QString &query)
{
    QScopedPointer<PlSqlScanner> scanner(new PlSqlScanner(new StringReader(query)));
    scanner->getNextToken();

    return scanner->getTokenLexeme().toUpper();
}

bool PlSqlParseHelper::isDml(const QString &query)
{
    QString firstTokenLexeme = PlSqlParseHelper::getFirstLexeme(query);

    return PlSqlParseHelper::isDmlStartLexeme(firstTokenLexeme);
}

bool PlSqlParseHelper::isDmlStartLexeme(const QString &lexeme)
{
    return lexeme.compare("SELECT", Qt::CaseInsensitive) == 0 ||
            lexeme.compare("UPDATE", Qt::CaseInsensitive) == 0 ||
            lexeme.compare("INSERT", Qt::CaseInsensitive) == 0 ||
            lexeme.compare("DELETE", Qt::CaseInsensitive) == 0 ||
            lexeme.compare("MERGE", Qt::CaseInsensitive) == 0 ||
            lexeme.compare("WITH", Qt::CaseInsensitive) == 0;
}

bool PlSqlParseHelper::isAnonymousBlockStartLexeme(const QString &lexeme)
{
    return lexeme.compare("BEGIN", Qt::CaseInsensitive) == 0 ||
            lexeme.compare("DECLARE", Qt::CaseInsensitive) == 0;
}

void PlSqlParseHelper::getNextQueryPos(const QString &query, int startFromPos, int *queryStartPos, int *queryEndPos)
{
    QScopedPointer<PlSqlScanner> scanner(new PlSqlScanner(new StringReader(query, startFromPos)));

    //skip to first keyword
    int token;
    do{
        token = scanner->getNextToken();
    }while(token>=NON_LITERAL_START_IX && token!=PLS_ID && token!=PLS_E_O_F);

    if(token==PLS_E_O_F || token==PLS_ERROR){
        *queryStartPos=-1;
        *queryEndPos=-1;
        return;
    }

    *queryStartPos = scanner->getTokenStartPos() + startFromPos;

    bool isPlSql=false;

    if(scanner->getTokenLexeme().compare("CREATE", Qt::CaseInsensitive)==0){
        token = scanner->getNextToken();
        if(scanner->getTokenLexeme().compare("OR", Qt::CaseInsensitive)==0){
            token = scanner->getNextToken();
            Q_ASSERT(scanner->getTokenLexeme().compare("REPLACE", Qt::CaseInsensitive)==0);
            token = scanner->getNextToken();
        }

        isPlSql = DbUtil::isPLSQLProgramUnit(scanner->getTokenLexeme());
    }else if(PlSqlParseHelper::isAnonymousBlockStartLexeme(scanner->getTokenLexeme())){
        isPlSql = true;
    }

    if(token == PLS_E_O_F){
        *queryEndPos = scanner->getTokenEndPos() + startFromPos;
        return;
    }

    //if isPlSql=true then scan to semicolon (;) followed by slash (/) or slash at the beginning of line
    //otherwise scan to semicolon (;)
    bool sawSemicolon=false;
    do{
        token = scanner->getNextToken();

        if(token==PLS_SEMI){
            sawSemicolon=true;
            if(!isPlSql){
                break;
            }
        }else if(sawSemicolon){
            Q_ASSERT(isPlSql);
            sawSemicolon=false;

            if(token==PLS_DIVIDE){
                break;
            }
        }else if(token==PLS_DIVIDE && scanner->getCurrentLine().trimmed()=="/"){ //slash as the only symbol of line
            break;
        }
    }while(token != PLS_E_O_F);

    *queryEndPos = scanner->getTokenEndPos() + startFromPos;
}

void PlSqlParseHelper::findObjectName(PlSqlTreeBuilder *treeBuilder, QString *schema, QString *name, const QString &defaultSchema)
{
    *schema = QString();
    *name = QString();

    ParseTreeNode *objectNameNode = treeBuilder->findNode(treeBuilder->getRootNode(), R_OBJECT_NAME, true);

    if(objectNameNode){
        if(objectNameNode->children.size()==1){
            *name = objectNameNode->children.at(0)->children.at(0)->tokenInfo->lexeme;
        }else{
            Q_ASSERT(objectNameNode->children.size()>=3);
            *schema = objectNameNode->children.at(0)->children.at(0)->tokenInfo->lexeme;
            *name = objectNameNode->children.at(2)->children.at(0)->tokenInfo->lexeme;
        }
    }

    if((*schema).isEmpty()){
        *schema = defaultSchema;
    }

    *schema = PlSqlParseHelper::cleanIdentifier(*schema);
    *name = PlSqlParseHelper::cleanIdentifier(*name);
}

/*
void PlSqlParseHelper::findObjectName(const QString &query, QString *schema, QString *name, const QString &defaultSchema)
{
    StringReader *reader = new StringReader(query);
    bool success;
    QScopedPointer<PlSqlTreeBuilder> treeBuilder(PlSqlParseHelper::createParseTree(reader, &success));
    PlSqlTokenFinder::findObjectName(treeBuilder.data(), schema, name);
    if(schema->isEmpty()){
        *schema = PlSqlParseHelper::cleanIdentifier(defaultSchema);
    }
}*/

QString PlSqlParseHelper::cleanIdentifier(const QString &identifier)
{
    if(identifier.startsWith('"') && identifier.endsWith('"')){
        return identifier.mid(1, identifier.size()-2);
    }else{
        return identifier.toUpper();
    }
}

PlSqlTreeBuilder *PlSqlParseHelper::createParseTree(TextReaderBase *reader, bool *success)
{
    QScopedPointer<CodeParser> parser(CodeParserFactory::createParser("plsql", reader));
    PlSqlTreeBuilder *treeBuilder = new PlSqlTreeBuilder();
    parser->setReduceListener(treeBuilder);
    bool parsed = parser->parse();

    if(success){
        *success = parsed;
    }

    return treeBuilder;
}

void PlSqlParseHelper::findTableNameInSelectQuery(const QString &query, QString *schemaName, QString *tableName, QString *dblink, const QString &defaultSchemaName)
{
    QScopedPointer<PlSqlScanner> scanner(new PlSqlScanner(new StringReader(query)));

    int token;
    do{
        token = scanner->getNextToken();

        if(token == PLS_LPAREN){
            bool foundMatching = PlSqlParseHelper::readToMatchingParentheses(scanner.data());

            if(!foundMatching){ //something is wrong
                return;
            }
        }else if(scanner->getTokenLexeme().compare("FROM", Qt::CaseInsensitive)==0){ //found first not nested FROM keyword. next comes table name
            PlSqlParseHelper::readTableName(scanner.data(), schemaName, tableName, dblink, defaultSchemaName);
            qDebug() << "schemaName =" << *schemaName << ", tableName =" << *tableName << ", dblink =" << *dblink;
            break;
        }

    }while(token!=PLS_E_O_F);
}

void PlSqlParseHelper::readTableName(PlSqlScanner *scanner, QString *schemaName, QString *tableName, QString *dblink, const QString &defaultSchemaName)
{
    int token = scanner->getNextToken();

    if(token == PLS_ID || token == PLS_DOUBLEQUOTED_STRING){
        *tableName = scanner->getTokenLexeme();
    }

    token = scanner->getNextToken();
    if(token != PLS_DOT){
        *schemaName = defaultSchemaName;
    }else{
        token = scanner->getNextToken();
        if(token == PLS_ID || token == PLS_DOUBLEQUOTED_STRING){
            *schemaName = *tableName;
            *tableName = scanner->getTokenLexeme();
        }

        token = scanner->getNextToken();
    }

    if(token==PLS_AT_SIGN){
        *dblink = PlSqlParseHelper::readMultiPartNameAsOne(scanner);
    }

    *schemaName = PlSqlParseHelper::cleanIdentifier(*schemaName);
    *tableName = PlSqlParseHelper::cleanIdentifier(*tableName);
}

QString PlSqlParseHelper::readMultiPartNameAsOne(PlSqlScanner *scanner)
{
    return PlSqlParseHelper::readMultiPartName(scanner).join(".");
}

QStringList PlSqlParseHelper::readMultiPartName(PlSqlScanner *scanner)
{
    QStringList parts;

    int token = scanner->getNextToken();
    while(token == PLS_ID || token == PLS_DOUBLEQUOTED_STRING){
        parts.append(scanner->getTokenLexeme());

        token = scanner->getNextToken();
        if(token != PLS_DOT){
            break;
        }

        token = scanner->getNextToken();
    }

    return parts;
}

void PlSqlParseHelper::prepareViewForEditing(QString &query)
{
    QString strToReplace = QString("CREATE OR REPLACE FORCE VIEW");
    int forceClauseIx = query.indexOf(strToReplace);
    if(forceClauseIx == 0){
        query.replace(0, strToReplace.length(), "CREATE OR REPLACE VIEW");
    }

    //now remove list of columns in parentheses
    QScopedPointer<PlSqlScanner> scanner(new PlSqlScanner(new StringReader(query)));

    int token;

    int firstOpeningParenIx = -1;
    int firstClosingParenIx = -1;

    do{
        token = scanner->getNextToken();

        if(token == PLS_LPAREN && firstOpeningParenIx==-1){
            firstOpeningParenIx = scanner->getTokenStartPos();

            bool foundMatching = PlSqlParseHelper::readToMatchingParentheses(scanner.data());

            if(foundMatching){
                firstClosingParenIx = scanner->getTokenEndPos();
            }else{ //something is wrong
                break;
            }
        }

    }while(token != PLS_E_O_F && token != PLS_ERROR);

    if(firstOpeningParenIx != -1 && firstClosingParenIx !=-1){
        query.remove(firstOpeningParenIx, firstClosingParenIx - firstOpeningParenIx);

        if(query.length() > firstOpeningParenIx + 1 && query.mid(firstOpeningParenIx-1, 2) == "  "){ //replace two spaces with one
            query.remove(firstOpeningParenIx-1, 1);
        }
    }
}

void PlSqlParseHelper::prepareTriggerForEditing(QString &query)
{
    QRegExp regExp("ALTER TRIGGER \\\".*\\\"\\.\\\".*\\\" (ENABLE|DISABLE)", Qt::CaseSensitive, QRegExp::RegExp2);

    query.replace(regExp, "");

    query = query.trimmed();
}

bool PlSqlParseHelper::readToMatchingParentheses(PlSqlScanner *scanner)
{
    //this method will be called after meeting first parentheses, so setting nestingCount to 1
    int nestingCount = 1;

    int token;

    do{
        token = scanner->getNextToken();

        if(token == PLS_LPAREN){
            ++nestingCount;
        }else if(token == PLS_RPAREN){
            --nestingCount;

            if(nestingCount == 0){ //found matching parenthesis
                return true;
            }
        }

    }while(token != PLS_E_O_F);

    return false;
}

int PlSqlParseHelper::extractPlSqlErrorCode(const QString &errorMessage)
{
    QString errMsg = errorMessage;

    bool removedPrefix = false;
    QStringList prefixesToRemove = QStringList() << "PL/SQL:" << "PLS";
    foreach(const QString &prefixToRemove, prefixesToRemove){
        if(errMsg.startsWith(prefixToRemove)){
            errMsg.remove(0, prefixToRemove.size());
            removedPrefix = true;
            break;
        }
    }

    if(!removedPrefix){
        return 0;
    }

    //now find first colon
    int colonIx = errMsg.indexOf(':');
    if(colonIx!=-1){
        QString descriptiveErrorCode=errMsg.mid(0, colonIx); //for example ORA-00942
        int dashIx=descriptiveErrorCode.indexOf('-');
        if(dashIx!=-1){
            QString strErrorCode=descriptiveErrorCode.mid(dashIx+1); //keeping only numeric part: 00942
            bool canParseNumber;
            int errorCode=strErrorCode.toInt(&canParseNumber);
            if(canParseNumber){
                return errorCode;
            }else{
                return 0;
            }
        }else{
            return 0;
        }
    }else{
        return 0;
    }
}

/*QStringList PlSqlParseHelper::tokenizeName(const QString &objectName)
{
    QStringList parts;

    //now remove list of columns in parentheses
    QScopedPointer<PlSqlScanner> scanner(new PlSqlScanner(new StringReader(objectName)));

    int token;

    do{
        token = scanner->getNextToken();

        if(token == PLS_ID || token < NON_LITERAL_START_IX || token == PLS_DOUBLEQUOTED_STRING){
            parts.append(PlSqlParseHelper::cleanIdentifier(scanner->getTokenLexeme()));
        }

    }while(token != PLS_E_O_F && token != PLS_ERROR);

    return parts;
}*/

PlSqlParseHelper::PlSqlParseHelper()
{
}
