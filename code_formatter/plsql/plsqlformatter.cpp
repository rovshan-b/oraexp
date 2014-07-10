#include "plsqlformatter.h"
#include "code_parser/stringreader.h"
#include "code_parser/plsql/plsqlscanner.h"
#include "code_parser/plsql/plsqlparsingtable.h"
#include "util/strutil.h"
#include <QScopedPointer>

QString PlSqlFormatter::strTab = QString("   ");

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
const static int defineKeyword = parsingTable->getKeywordIx("DEFINE");
const static int measuresKeyword = parsingTable->getKeywordIx("MEASURES");

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
const static int thenKeyword = parsingTable->getKeywordIx("THEN");
const static int elseKeyword = parsingTable->getKeywordIx("ELSE");
const static int endKeyword = parsingTable->getKeywordIx("END");

const static int declareKeyword = parsingTable->getKeywordIx("DECLARE");
const static int beginKeyword = parsingTable->getKeywordIx("BEGIN");

const static int ifKeyword = parsingTable->getKeywordIx("IF");
const static int elsifKeyword = parsingTable->getKeywordIx("ELSIF");
//const static int elseKeyword = parsingTable->getKeywordIx("ELSE");

PlSqlFormatter::PlSqlFormatter()
{
}

QString PlSqlFormatter::format(const QString &code)
{
    scanner = new PlSqlScanner(new StringReader(code));
    QScopedPointer<PlSqlScanner> scopedScanner(scanner);

    //const static int selectKeyword = PlSqlParsingTable::getInstance()->getKeywordIx("SELECT");

    int token;

    do{
        token = scanner->getNextToken(false);

        if(token == selectKeyword){
            formatSelectStatement(false);
        }else if(token == PLS_LPAREN){
            formatParameterList();
        }else{
            formatDefaultToken(token);
        }

        indents.clear();

    }while(token != PLS_E_O_F);

    return result;
}

bool PlSqlFormatter::checkPrevToken(int token)
{
    return (prevTokenList.size() > 0 && prevTokenList[prevTokenList.size()-1] == token);
}

bool PlSqlFormatter::isPrevKeyword()
{
    int size = prevTokenList.size();
    if(size == 0){
        return false;
    }

    int prevToken = prevTokenList[size-1];

    if(prevToken < NON_LITERAL_START_IX && PlSqlParsingTable::getInstance()->getKeywordLexeme(prevToken).length() >= MIN_KEYWORD_LENGTH){
        return true;
    }

    return false;
}

int PlSqlFormatter::getPrevToken()
{
    return (prevTokenList.size() > 0 ? prevTokenList[prevTokenList.size()-1] : -1);
}

void PlSqlFormatter::indent(QString &str)
{
    foreach(const QString &indent, indents){
        str.append(indent);
    }
}

bool PlSqlFormatter::indentToEnd(QString &str)
{
    int lastNewlinePos = qMax(str.lastIndexOf('\n'), 0);

    int currentLinePos = qMax(str.length() - (lastNewlinePos + (lastNewlinePos==0 ? 0 : 1)), 0);

    int totalIndentLength = 0;
    foreach(const QString &indent, indents){
        totalIndentLength += indent.length();
    }

    if(currentLinePos > totalIndentLength){
        QChar space(' ');
        QString newIndent;
        for(int i=0; i<(currentLinePos - totalIndentLength); ++i){
            newIndent.append(space);
        }
        //str.append(newIndent);
        indents.push(newIndent);

        return true;
    }

    return false;

}

void PlSqlFormatter::increaseIndenting()
{
    indents.push(PlSqlFormatter::strTab);
}

void PlSqlFormatter::unindent()
{
    if(!indents.isEmpty()){
        indents.pop();
    }
}

bool PlSqlFormatter::formatGenericConstruct(int token, bool nested)
{
    bool formatted = false;

    if(token == selectKeyword){
        formatSelectStatement(nested);
        formatted = true;
    }else if(token == PLS_LPAREN){
        formatParameterList();
        formatted = true;
    }

    return formatted;
}

void PlSqlFormatter::formatSelectStatement(bool nested)
{
    Q_ASSERT(scanner->getTokenLexeme().compare("SELECT", Qt::CaseInsensitive) == 0);

    formatDefaultToken(selectKeyword);
    int token;

    prevTokenList.append(selectKeyword);

    SelectStatementSection section = SelectList;

    //QStack<QString> indents(initialIndents);
    increaseIndenting();

    bool formattedToEnd = indentToEnd(result);
    //add one more space
    indents.last().append(" ");

    do{
        token = scanner->getNextToken(false);

        //int prevToken = getPrevToken();

        bool formatted = formatGenericConstruct(token, true);
        if(formatted){
            prevTokenList.clear();
            continue;
        }

        if((token == intoKeyword) && section == SelectList){
            section = IntoList;
        }else if(token == fromKeyword){
            section = TableList;
            unindent();
        }else if(token == whereKeyword){
            section = Where;
        }else if(token == byKeyword){
            if(checkPrevToken(groupKeyword)){
                section = GroupBy;
            }else if(checkPrevToken(orderKeyword)){
                section = OrderBy;
            }
        }else if(token == havingKeyword && section == GroupBy){
            section = Having;
        }

        formatDefaultToken(token);

        if(token == PLS_COMMA &&
                (section == SelectList || section == IntoList)){
            result.append('\n');
            indent(result);
        }

        prevTokenList.append(token);

        if(nested && token == PLS_RPAREN){
            break;
        }

    }while(token != PLS_E_O_F && token != PLS_SEMI);

    if(formattedToEnd){
        unindent();
    }

    unindent();
}

bool PlSqlFormatter::formatParameterList(int nestingLevel)
{
    int token;

    //int prevToken = getPrevToken();
    formatDefaultToken(PLS_LPAREN);
    prevTokenList.append(PLS_LPAREN);
    indentToEnd(result);

    int commaCount = 0;
    bool multiline = false;
    bool innerMultiline = false;

    do{
        token = scanner->getNextToken(false);
        //int prevToken = getPrevToken();

        switch(token){
        case PLS_LPAREN:
            innerMultiline = formatParameterList(nestingLevel+1);
            break;
        case PLS_RPAREN:

            if(multiline || innerMultiline){
                result.append('\n');
                if(indents.size() > 0){
                    indents.last().chop(1); //so that closing parenthesis is on same column as opening
                }
                indent(result);
            }

            unindent();

            formatDefaultToken(token);

            return multiline;
            //break;
        default:
            if(token == selectKeyword){
                formatSelectStatement(true);
                return multiline;
            }

            bool formatted = formatGenericConstruct(token, true);
            if(!formatted){
                formatDefaultToken(token);

                if(token == PLS_COMMA){
                    ++commaCount;
                    if(commaCount >= 3){
                        result.append('\n');
                        multiline = true;
                        indent(result);
                        commaCount = 0;
                    }
                }
            }
            break;
        }

        prevTokenList.append(token);

    }while(token != PLS_E_O_F && token != PLS_SEMI);

    return multiline;
}

void PlSqlFormatter::formatDefaultToken(int token)
{
    int prevToken = getPrevToken();

    QList<int> newlineBeforeList;
    newlineBeforeList << selectKeyword
                      << bulkKeyword
                      << intoKeyword
                      << fromKeyword
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
                      << declareKeyword
                      << beginKeyword
                      << endKeyword
                      << defineKeyword
                      << measuresKeyword
                         ;

    QList<int> newlineAfterList;
    newlineAfterList << PLS_SEMI
                     << PLS_SL_COMMENT
                     << PLS_ML_COMMENT
                     << declareKeyword
                     << beginKeyword
                     << intoKeyword
                     << thenKeyword
                     << elseKeyword
                     << defineKeyword
                        ;

    QList<int> indentAfter;
    indentAfter << declareKeyword
                << beginKeyword
                << ifKeyword
                << caseKeyword
                << whenKeyword
                << elseKeyword;

    QList<int> unindentBefore;
    unindentBefore << endKeyword
                   << elsifKeyword
                   << elseKeyword
                   << whenKeyword
                   << unionKeyword
                   << intersectKeyword
                   << minusKeyword;

    if(unindentBefore.contains(token)){
        unindent();
    }

    if(newlineBeforeList.contains(token) &&
            !result.isEmpty() &&
            !newlineBeforeList.contains(prevToken) &&
            !endsWith(result, QList<QChar>() << '\n' << '(')){
        result.append('\n');
        indent(result);
    }
    bool willAddNewline = newlineAfterList.contains(token);

    //bool indentedToEnd = false;
    //if(indentAfter.contains(token)){
    //    indentedToEnd = PlSqlFormatter::indentToEnd(result, indents);
    //}

    QList<int> spacesAtBothSides;
    spacesAtBothSides << PLS_PLUS << PLS_MINUS << PLS_DIVIDE << PLS_EQ << PLS_PERCENTAGE << PLS_ASSIGN << PLS_ARROW
                      << PLS_VERTBAR << PLS_DOUBLEVERTBAR << PLS_NOT_EQ << PLS_LESS_THAN << PLS_LESS_THAN_EQUAL
                      << PLS_GREATER_THAN << PLS_GREATER_THAN_EQUAL << PLS_QUESTION_MARK << PLS_ERROR;

    QList<int> spaceAfter;
    spaceAfter << PLS_COMMA;

    QList<int> noSpaceBefore;
    noSpaceBefore << PLS_RPAREN << PLS_DOT;

    QList<int> noSpaceAfter;
    noSpaceAfter << PLS_LPAREN << PLS_DOT;

    QChar lastChar = result.length() > 0 ? result.right(1)[0] : QChar();
    bool prependSpace = !lastChar.isNull() && !lastChar.isSpace() && lastChar!='\n' &&
                            !noSpaceBefore.contains(token) &&
                            !noSpaceAfter.contains(prevToken);

    if(spacesAtBothSides.contains(token)){
        if(prependSpace){
            result.append(' ');
        }
        result.append(scanner->getTokenLexeme());
        if(!willAddNewline){
            result.append(' ');
        }
    }else if(spaceAfter.contains(token)){
        result.append(scanner->getTokenLexeme());
        if(!willAddNewline){
            result.append(' ');
        }
    }else{
        if(prependSpace){
            result.append(' ');
        }
        result.append(scanner->getTokenLexeme());
    }

    if(indentAfter.contains(token)){
        //if(indentedToEnd){
        //    indents.last().append(PlSqlFormatter::strTab);
        //}else{
            increaseIndenting();
        //}
    }

    if(willAddNewline){
        result.append('\n');
        if(token != PLS_SEMI){
            indent(result);
        }
    }
}
