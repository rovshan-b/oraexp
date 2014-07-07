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
    QScopedPointer<PlSqlScanner> scanner(new PlSqlScanner(new StringReader(code)));

    QString result;

    //const static int selectKeyword = PlSqlParsingTable::getInstance()->getKeywordIx("SELECT");

    int token;

    QStack<QString> indents;

    do{
        token = scanner->getNextToken(false);

        if(token == selectKeyword){
            PlSqlFormatter::formatSelectStatement(result, scanner.data(), false, indents);
        }else{
            PlSqlFormatter::formatDefaultToken(result, token, -1, scanner.data(), indents);
        }

        indents.clear();

    }while(token != PLS_E_O_F);

    return result;
}

bool PlSqlFormatter::checkPrevToken(const QList<int> &prevTokenList, int token)
{
    return (prevTokenList.size() > 0 && prevTokenList[prevTokenList.size()-1] == token);
}

bool PlSqlFormatter::isPrevKeyword(const QList<int> &prevTokenList)
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

int PlSqlFormatter::getPrevToken(const QList<int> &prevTokenList)
{
    return (prevTokenList.size() > 0 ? prevTokenList[prevTokenList.size()-1] : -1);
}

void PlSqlFormatter::indent(QString &str, const QStack<QString> &indents)
{
    foreach(const QString &indent, indents){
        str.append(indent);
    }
}

bool PlSqlFormatter::indentToEnd(QString &str, QStack<QString> &indents)
{
    int lastNewlinePos = qMax(str.lastIndexOf('\n'), 0);

    int currentLinePos = qMax(str.length() - lastNewlinePos - 1, 0);

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

void PlSqlFormatter::increaseIndenting(QStack<QString> &indents)
{
    indents.push(PlSqlFormatter::strTab);
}

void PlSqlFormatter::unindent(QStack<QString> &indents)
{
    if(!indents.isEmpty()){
        indents.pop();
    }
}

bool PlSqlFormatter::formatGenericConstruct(QString &result, int token, CodeScanner *scanner, QList<int> &prevTokenList, QStack<QString> &indents, bool nested)
{
    bool formatted = false;

    if(token == selectKeyword){
        PlSqlFormatter::formatSelectStatement(result, scanner, nested, indents);
        formatted = true;
    }else if(token == PLS_LPAREN){
        PlSqlFormatter::formatParameterList(result, scanner, prevTokenList, indents);
        formatted = true;
    }

    return formatted;
}

void PlSqlFormatter::formatSelectStatement(QString &result, CodeScanner *scanner, bool nested, QStack<QString> &indents)
{
    Q_ASSERT(scanner->getTokenLexeme().compare("SELECT", Qt::CaseInsensitive) == 0);

    result.append(scanner->getTokenLexeme());
    //result.append(' ');

    int token;
    QList<int> prevTokenList;
    bool addToPrevList;

    prevTokenList.append(selectKeyword);

    SelectStatementSection section = SelectList;

    //QStack<QString> indents(initialIndents);
    PlSqlFormatter::increaseIndenting(indents);

    bool formattedToEnd = PlSqlFormatter::indentToEnd(result, indents);
    //add one more space
    indents.last().append("  ");

    do{
        token = scanner->getNextToken(false);
        addToPrevList = true;

        int prevToken = PlSqlFormatter::getPrevToken(prevTokenList);

        bool formatted = PlSqlFormatter::formatGenericConstruct(result, token, scanner, prevTokenList, indents, true);
        if(formatted){
            prevTokenList.clear();
            continue;
        }

        if((token == intoKeyword) && section == SelectList){
            section = IntoList;
        }else if(token == fromKeyword){
            section = TableList;
            PlSqlFormatter::unindent(indents);
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

        PlSqlFormatter::formatDefaultToken(result, token, prevToken, scanner, indents);

        if(token == PLS_COMMA &&
                (section == SelectList || section == IntoList)){
            result.append('\n');
            PlSqlFormatter::indent(result, indents);
        }

        if(addToPrevList){
            prevTokenList.append(token);
        }

        if(nested && token == PLS_RPAREN){
            break;
        }

    }while(token != PLS_E_O_F && token != PLS_SEMI);

    if(formattedToEnd){
        PlSqlFormatter::unindent(indents);
    }

    PlSqlFormatter::unindent(indents);
}

bool PlSqlFormatter::formatParameterList(QString &result, CodeScanner *scanner, QList<int> &prevTokenList, QStack<QString> &indents, int nestingLevel)
{
    int token;

    int prevToken = PlSqlFormatter::getPrevToken(prevTokenList);
    PlSqlFormatter::formatDefaultToken(result, PLS_LPAREN, prevToken, scanner, indents);
    prevTokenList.append(PLS_LPAREN);
    PlSqlFormatter::indentToEnd(result, indents);

    int commaCount = 0;
    bool multiline = false;
    bool innerMultiline = false;

    do{
        token = scanner->getNextToken(false);
        prevToken = PlSqlFormatter::getPrevToken(prevTokenList);

        switch(token){
        case PLS_LPAREN:
            innerMultiline = PlSqlFormatter::formatParameterList(result, scanner, prevTokenList, indents, nestingLevel+1);
            break;
        case PLS_RPAREN:

            if(multiline || innerMultiline){
                result.append('\n');
                PlSqlFormatter::indent(result, indents);
            }

            PlSqlFormatter::unindent(indents);

            PlSqlFormatter::formatDefaultToken(result, token, prevToken, scanner, indents);

            return multiline;
            //break;
        default:
            if(token == selectKeyword){
                PlSqlFormatter::formatSelectStatement(result, scanner, true, indents);
                return multiline;
            }

            bool formatted = PlSqlFormatter::formatGenericConstruct(result, token, scanner, prevTokenList, indents, true);
            if(!formatted){
                PlSqlFormatter::formatDefaultToken(result, token, prevToken, scanner, indents);

                if(token == PLS_COMMA){
                    ++commaCount;
                    if(commaCount >= 3){
                        result.append('\n');
                        multiline = true;
                        PlSqlFormatter::indent(result, indents);
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

void PlSqlFormatter::formatDefaultToken(QString &result, int token, int prevToken, CodeScanner *scanner, QStack<QString> &indents)
{
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
    newlineAfterList << PLS_SEMI
                     << PLS_SL_COMMENT
                     << PLS_ML_COMMENT
                     << declareKeyword
                     << beginKeyword
                     << intoKeyword
                     << thenKeyword
                     << elseKeyword
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
                   << whenKeyword;

    if(unindentBefore.contains(token)){
        PlSqlFormatter::unindent(indents);
    }

    if(newlineBeforeList.contains(token) && !result.isEmpty() && !endsWithNewline(result)){
        result.append('\n');
        PlSqlFormatter::indent(result, indents);
    }
    bool willAddNewline = newlineAfterList.contains(token);

    bool indentedToEnd = false;
    if(indentAfter.contains(token)){
        indentedToEnd = PlSqlFormatter::indentToEnd(result, indents);
    }

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
        if(indentedToEnd){
            indents.last().append(PlSqlFormatter::strTab);
        }else{
            PlSqlFormatter::increaseIndenting(indents);
        }
    }

    if(willAddNewline){
        result.append('\n');
        if(token != PLS_SEMI){
            PlSqlFormatter::indent(result, indents);
        }
    }
}
