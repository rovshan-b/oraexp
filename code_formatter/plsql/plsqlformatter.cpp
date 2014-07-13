#include "plsqlformatter.h"
#include "code_parser/stringreader.h"
#include "code_parser/plsql/plsqlscanner.h"
#include "code_parser/plsql/plsqlparsingtable.h"
#include "beans/tokeninfo.h"
#include "util/strutil.h"
#include <QScopedPointer>

QString PlSqlFormatter::strTab = QString("   ");

PlSqlParsingTable *parsingTable = PlSqlParsingTable::getInstance();

const static int selectKeyword = parsingTable->getKeywordIx("SELECT");
const static int intoKeyword = parsingTable->getKeywordIx("INTO");
const static int fromKeyword = parsingTable->getKeywordIx("FROM");
const static int whereKeyword = parsingTable->getKeywordIx("WHERE");
const static int groupKeyword = parsingTable->getKeywordIx("GROUP");
const static int havingKeyword = parsingTable->getKeywordIx("HAVING");
const static int orderKeyword = parsingTable->getKeywordIx("ORDER");
const static int byKeyword = parsingTable->getKeywordIx("BY");
const static int modelKeyword = parsingTable->getKeywordIx("MODEL");
const static int offsetKeyword = parsingTable->getKeywordIx("OFFSET");
const static int fetchKeyword = parsingTable->getKeywordIx("FETCH");
const static int updateKeyword = parsingTable->getKeywordIx("UPDATE");
const static int forKeyword = parsingTable->getKeywordIx("FOR");

PlSqlFormatter::PlSqlFormatter() :
    currTokenInfo(0),
    token(-1)
{
}

PlSqlFormatter::~PlSqlFormatter()
{
    qDeleteAll(prevTokenList);
}

QString PlSqlFormatter::format(const QString &code)
{
    scanner = new PlSqlScanner(new StringReader(code));
    QScopedPointer<PlSqlScanner> scopedScanner(scanner);

    //const static int selectKeyword = PlSqlParsingTable::getInstance()->getKeywordIx("SELECT");

    do{
        readNextToken();

        if(token == selectKeyword){
            formatSelectStatement(false);
        }else if(token == PLS_LPAREN){
            formatParameterList();
        }else{
            formatDefaultToken();
        }

        indents.clear();

    }while(token != PLS_E_O_F);

    return result;
}

bool PlSqlFormatter::checkPrevToken(int token)
{
    TokenInfo *tokenInfo = getPrevToken();
    return tokenInfo != 0 && tokenInfo->tokenOrRuleId == token;
}

bool PlSqlFormatter::isPrevKeyword()
{
    TokenInfo *tokenInfo = getPrevToken();
    if(tokenInfo == 0){
        return false;
    }

    if(tokenInfo->tokenOrRuleId < NON_LITERAL_START_IX &&
            tokenInfo->lexeme.length() >= MIN_KEYWORD_LENGTH){
        return true;
    }

    return false;
}

TokenInfo* PlSqlFormatter::getPrevToken()
{
    return (prevTokenList.size() > 1 ? prevTokenList[prevTokenList.size()-2] : 0);
}

void PlSqlFormatter::indent(QString &str)
{
    foreach(const QString &indent, indents){
        str.append(indent);
    }
}

bool PlSqlFormatter::indentToEnd(QString &str, bool additionalSpace)
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

        if(additionalSpace){
            newIndent.append(space);
        }

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

    formatDefaultToken();

    SelectStatementSection section = SelectList;

    increaseIndenting();

    bool formattedToEnd = indentToEnd(result, true);

    do{
        readNextToken();

        bool formatted = formatGenericConstruct(token, true);
        if(formatted){
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
        }else if(token == modelKeyword){
            section = Model;
        }else if(token == offsetKeyword || token == fetchKeyword){
            section = RowLimiting;
        }else if(token == updateKeyword && checkPrevToken(forKeyword)){
            section = ForUpdate;
        }

        formatDefaultToken();

        if(token == PLS_COMMA &&
                (section == SelectList || section == IntoList)){
            result.append('\n');
            indent(result);
        }

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
    formatDefaultToken();
    indentToEnd(result);

    int commaCount = 0;
    bool multiline = false;
    bool innerMultiline = false;

    do{
        readNextToken();

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

            formatDefaultToken();

            return multiline;
            //break;
        default:
            if(token == selectKeyword){
                formatSelectStatement(true);
                return multiline;
            }

            bool formatted = formatGenericConstruct(token, true);
            if(!formatted){
                formatDefaultToken();

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

    }while(token != PLS_E_O_F && token != PLS_SEMI);

    return multiline;
}

void PlSqlFormatter::formatDefaultToken()
{
    QString lexeme = currTokenInfo->lexeme;

    QList<CodeFormatterAction*> actions = settings.actionsForToken(token, lexeme);

    QList<CodeFormatterAction*> prevTokenActions;
    TokenInfo *prevTokenInfo = getPrevToken();
    if(prevTokenInfo != 0){
        prevTokenActions = settings.actionsForToken(prevTokenInfo->tokenOrRuleId, prevTokenInfo->lexeme);
    }

    foreach(CodeFormatterAction *action, actions){
        if(action->getSequence() == CodeFormatterAction::Before){
            applyAction(action, prevTokenActions);
        }
    }

    QChar lastChar = result.length() > 0 ? result.right(1)[0] : QChar();
    bool prependSpace = !lastChar.isNull() && !lastChar.isSpace() && lastChar!='\n' &&
                            !containsAction(actions, CodeFormatterAction::Before, CodeFormatterAction::NoSpace) &&
                            !containsAction(prevTokenActions, CodeFormatterAction::After, CodeFormatterAction::NoSpace);

    if(prependSpace){
        result.append(' ');
    }

    result.append(lexeme);

    foreach(CodeFormatterAction *action, actions){
        if(action->getSequence() == CodeFormatterAction::After){
            applyAction(action, prevTokenActions);
        }
    }
}

void PlSqlFormatter::applyAction(CodeFormatterAction *action, const QList<CodeFormatterAction *> &prevTokenActions)
{
    switch(action->getType()){
    case CodeFormatterAction::Newline:
        if(action->getSequence() == CodeFormatterAction::Before){
            if(!result.isEmpty() &&
                    (action->boolValue("repeat")==true ||
                     !containsAction(prevTokenActions, CodeFormatterAction::Any, CodeFormatterAction::Newline)) &&
                    !endsWith(result, action->charList("not_after"))){
                result.append('\n');
                indent(result);
            }
        }else{
            result.append('\n');
            if(token != PLS_SEMI){
                indent(result);
            }
        }
        break;
    case CodeFormatterAction::Space:
        result.append(' ');
        break;
    case CodeFormatterAction::NoSpace:
        break;
    case CodeFormatterAction::Indent:
        increaseIndenting();
        break;
    case CodeFormatterAction::Unindent:
        unindent();
        break;
    default:
        Q_ASSERT(false);
        break;
    }
}

bool PlSqlFormatter::containsAction(const QList<CodeFormatterAction *> &actions, CodeFormatterAction::ActionSequence sequence, CodeFormatterAction::ActionType type) const
{
    foreach(CodeFormatterAction *action, actions){
        if((action->getSequence() == sequence ||
            sequence == CodeFormatterAction::BeforeAndAfter ||
            sequence == CodeFormatterAction::Any) &&
                action->getType() == type){
            return true;
        }
    }

    return false;
}

void PlSqlFormatter::readNextToken()
{
    token = scanner->getNextToken(false);
    currTokenInfo = scanner->createTokenInfo(token);

    prevTokenList.append(currTokenInfo);
}
