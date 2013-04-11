#include "plsqlparsehelper.h"
#include "plsqlscanner.h"
#include "../stringreader.h"
#include "plsqltokens.h"
#include "code_parser/plsql/plsqlparsingtable.h"
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
            if(lastLexeme=="CREATE"){ //no bind param prompting for DDL
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

    if(firstTokenLexeme=="SELECT" ||
            firstTokenLexeme=="UPDATE" ||
            firstTokenLexeme=="INSERT" ||
            firstTokenLexeme=="DELETE" ||
            firstTokenLexeme=="MERGE" ||
            firstTokenLexeme=="WITH"){
        return true;
    }else{
        return false;
    }
}

PlSqlParseHelper::PlSqlParseHelper()
{
}
