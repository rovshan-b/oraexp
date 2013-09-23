#include "plsqlparsehelper.h"
#include "plsqlscanner.h"
#include "../stringreader.h"
#include "../textcursorreader.h"
#include "../codeparser.h"
#include "../codeparserfactory.h"
#include "plsqltokens.h"
#include "plsqlparsingtable.h"
#include "plsqltokenfinder.h"
#include "plsqlcodecollapsepositionfinder.h"
#include "plsqltreebuilder.h"
#include "beans/codecollapseposition.h"
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

void PlSqlParseHelper::findObjectName(const QString &query, QString *schema, QString *name, const QString &defaultSchema)
{
    StringReader *reader = new StringReader(query);
    bool success;
    QScopedPointer<PlSqlTreeBuilder> treeBuilder(PlSqlParseHelper::createParseTree(reader, &success));
    if(success){
        PlSqlTokenFinder::findObjectName(treeBuilder.data(), schema, name);
        if(schema->isEmpty()){
            *schema = defaultSchema;
        }

        *schema = PlSqlParseHelper::cleanIdentifier(*schema);
        *name = PlSqlParseHelper::cleanIdentifier(*name);
    }
}

QString PlSqlParseHelper::cleanIdentifier(const QString &identifier)
{
    if(identifier.startsWith('"') && identifier.endsWith('"')){
        return identifier.mid(1, identifier.size()-2);
    }else{
        return identifier.toUpper();
    }
}

QList<CodeCollapsePosition*> PlSqlParseHelper::findCodeCollapsePositions(const QTextCursor &cur)
{
    //TextCursorReader *reader = new TextCursorReader(cur);
    //QScopedPointer<PlSqlTreeBuilder> treeBuilder(PlSqlParseHelper::createParseTree(reader));

    PlSqlCodeCollapsePositionFinder codeCollapsePositionFinder;

    return codeCollapsePositionFinder.findCodeCollapsePositions(cur);
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


PlSqlParseHelper::PlSqlParseHelper()
{
}
