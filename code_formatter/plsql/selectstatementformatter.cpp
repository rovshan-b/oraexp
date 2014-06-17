#include "selectstatementformatter.h"
#include "code_parser/codescanner.h"
#include "code_parser/plsql/plsqltokens.h"
#include "code_parser/plsql/plsqlparsingtable.h"
#include "defaulttokenformatter.h"
#include "plsqlformatter.h"

SelectStatementFormatter::SelectStatementFormatter()
{
}

QString SelectStatementFormatter::format(CodeScanner *scanner, bool nested)
{
    Q_ASSERT(scanner->getTokenLexeme().compare("SELECT", Qt::CaseInsensitive) == 0);

    QString result(scanner->getTokenLexeme());

    int token;
    QList<int> prevTokenList;
    bool addToPrevList;

    PlSqlParsingTable *parsingTable = PlSqlParsingTable::getInstance();
    const static int selectKeyword = parsingTable->getKeywordIx("SELECT");
    const static int bulkKeyword = parsingTable->getKeywordIx("BULK");
    const static int intoKeyword = parsingTable->getKeywordIx("INTO");
    const static int fromKeyword = parsingTable->getKeywordIx("FROM");
    const static int whereKeyword = parsingTable->getKeywordIx("WHERE");
    const static int groupKeyword = parsingTable->getKeywordIx("GROUP");
    const static int havingKeyword = parsingTable->getKeywordIx("HAVING");
    const static int orderKeyword = parsingTable->getKeywordIx("ORDER");
    const static int byKeyword = parsingTable->getKeywordIx("BY");
    const static int startKeyword = parsingTable->getKeywordIx("START");
    const static int connectKeyword = parsingTable->getKeywordIx("CONNECT");
    const static int modelKeyword = parsingTable->getKeywordIx("MODEL");
    const static int unionKeyword = parsingTable->getKeywordIx("UNION");
    const static int intersectKeyword = parsingTable->getKeywordIx("INTERSECT");
    const static int minusKeyword = parsingTable->getKeywordIx("MINUS");
    const static int forKeyword = parsingTable->getKeywordIx("FOR");
    const static int partitionKeyword = parsingTable->getKeywordIx("PARTITION");

    const static int innerKeyword = parsingTable->getKeywordIx("INNER");
    const static int outerKeyword = parsingTable->getKeywordIx("OUTER");
    const static int leftKeyword = parsingTable->getKeywordIx("LEFT");
    const static int rightKeyword = parsingTable->getKeywordIx("RIGHT");
    const static int crossKeyword = parsingTable->getKeywordIx("CROSS");
    const static int naturalKeyword = parsingTable->getKeywordIx("NATURAL");
    const static int fullKeyword = parsingTable->getKeywordIx("FULL");

    const static int andKeyword = parsingTable->getKeywordIx("AND");
    const static int orKeyword = parsingTable->getKeywordIx("OR");

    const static int caseKeyword = parsingTable->getKeywordIx("CASE");
    const static int whenKeyword = parsingTable->getKeywordIx("WHEN");
    //const static int thenKeyword = parsingTable->getKeywordIx("THEN");
    const static int elseKeyword = parsingTable->getKeywordIx("ELSE");
    const static int endKeyword = parsingTable->getKeywordIx("END");


    QList<int> newlineBeforeList;
    newlineBeforeList << selectKeyword
                      << bulkKeyword
                      << intoKeyword
                      << whereKeyword
                      << groupKeyword
                      << havingKeyword
                      << orderKeyword
                      << startKeyword
                      << connectKeyword
                      << modelKeyword
                      << unionKeyword
                      << intersectKeyword
                      << minusKeyword
                      << forKeyword
                      << partitionKeyword
                      << innerKeyword
                      << outerKeyword
                      << leftKeyword
                      << rightKeyword
                      << crossKeyword
                      << naturalKeyword
                      << fullKeyword
                      << andKeyword
                      << orKeyword
                      << caseKeyword
                      << whenKeyword
                      << elseKeyword
                      << endKeyword
                         ;

    QList<int> newlineAfterList;
    newlineAfterList << PLS_COMMA
                     << PLS_SL_COMMENT
                     << PLS_ML_COMMENT
                     //<< thenKeyword
                        ;

    SelectStatementSection section = SelectList;

    int nestingLevel = 0;
    do{
        token = scanner->getNextToken(false);
        addToPrevList = true;

        switch(token){
        case PLS_LPAREN:
            ++nestingLevel;
            result.append(DefaultTokenFormatter::format(scanner));
            break;
        case PLS_RPAREN:
        {
            --nestingLevel;
            result.append(DefaultTokenFormatter::format(scanner));

            if(nestingLevel < 0){
                if(nested){
                    return result;
                }else{ //correct wrong nesting
                    nestingLevel = 0;
                }
            }

            break;
        }
        default:
        {
            if((token == intoKeyword) && section == SelectList){
                section = IntoList;
            }else if(token == fromKeyword){
                section = TableList;
            }else if(token == whereKeyword){
                section = Where;
            }else if(token == byKeyword){
                if(PlSqlFormatter::checkPrevToken(prevTokenList, groupKeyword)){
                    section = GroupBy;
                }else if(PlSqlFormatter::checkPrevToken(prevTokenList, orderKeyword)){
                    section = OrderBy;
                }
            }else if(token == havingKeyword && section == GroupBy){
                section = Having;
            }

            int prevToken = (prevTokenList.size() > 0 ? prevTokenList[prevTokenList.size()-1] : -1);

            if(newlineBeforeList.contains(token) && !newlineBeforeList.contains(prevToken)){
                result.append('\n');
            }

            if(token == selectKeyword){
                prevTokenList.clear();
                addToPrevList = false;
                result.append(SelectStatementFormatter::format(scanner, nestingLevel > 0));
            }else{
                result.append(DefaultTokenFormatter::format(scanner));
            }

            if(newlineAfterList.contains(token)){
                result.append('\n');
            }

            break;
        }
        }

        if(addToPrevList){
            prevTokenList.append(token);
        }

    }while(token != PLS_E_O_F && token != PLS_SEMI);

    return result;

}
