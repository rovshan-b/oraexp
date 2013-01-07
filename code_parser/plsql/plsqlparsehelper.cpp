#include "plsqlparsehelper.h"
#include "plsqlscanner.h"
#include "../stringreader.h"
#include "plsqltokens.h"
#include <QDebug>

QStringList PlSqlParseHelper::getBindParams(const QString &query)
{
    QStringList results;

    QScopedPointer<PlSqlScanner> scanner(new PlSqlScanner(new StringReader(query)));


    int token = PLS_E_O_F;
    bool waitingForName=false;

    do{
        token = scanner->getNextToken();

        if(token==PLS_COLON){ //waiting for bind variable name
            waitingForName=true;
        }else if(waitingForName && (token==PLS_ID || token==PLS_DOUBLEQUOTED_STRING)){
            waitingForName=false;

            //must keep single entry for non double quoted strings regardless of case
            //and must keep single entry for double quoted strings taking into account case
            QString paramName = scanner->getTokenLexeme();
            if(!results.contains(paramName,
                                 token==PLS_DOUBLEQUOTED_STRING ?
                                        Qt::CaseSensitive :
                                        Qt::CaseInsensitive)){
                results.append(token==PLS_DOUBLEQUOTED_STRING ? paramName : paramName.toUpper());
            }
        }else if(waitingForName){
            waitingForName=false;
        }

    }while(token!=PLS_E_O_F && token!=PLS_ERROR);

    qDebug() << results;

    return results;
}

PlSqlParseHelper::PlSqlParseHelper()
{
}
