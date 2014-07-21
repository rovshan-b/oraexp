#include "plsqlformatter.h"
#include "code_parser/stringreader.h"
#include "code_parser/plsql/plsqlscanner.h"
#include "code_parser/plsql/plsqlparsingtable.h"
#include "beans/tokeninfo.h"
#include "util/strutil.h"
#include <QScopedPointer>

#define DEFAULT_PARAM_COUNT_PER_LINE 3

QString PlSqlFormatter::strTab = QString("   ");

PlSqlParsingTable *parsingTable = PlSqlParsingTable::getInstance();

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

        if(!formatGenericConstruct()){
            formatDefaultToken();
        }

        //indents.clear();

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

TokenInfo* PlSqlFormatter::getPrevToken(bool ignoreWhitespace)
{
    int stepIntoList = 2;

    TokenInfo *prevToken = (prevTokenList.size() > (stepIntoList - 1) ? prevTokenList[prevTokenList.size()-stepIntoList] : 0);

    //skip comments
    if(ignoreWhitespace){
        while(prevToken != 0 && (prevToken->tokenOrRuleId == PLS_SL_COMMENT ||
                                 prevToken->tokenOrRuleId == PLS_ML_COMMENT)){
            ++stepIntoList;
            prevToken = (prevTokenList.size() > (stepIntoList - 1) ? prevTokenList[prevTokenList.size()-stepIntoList] : 0);
        }
    }

    return prevToken;
}

void PlSqlFormatter::indent(QString &str)
{
    foreach(const QString &indent, indents){
        str.append(indent);
    }
}

void PlSqlFormatter::indentToEnd(QString &str, bool additionalSpace)
{
    int lastNewlinePos = qMax(str.lastIndexOf('\n'), 0);

    int currentLinePos = qMax(str.length() - (lastNewlinePos + (lastNewlinePos==0 ? 0 : 1)), 0);

    if(additionalSpace){
        ++currentLinePos;
    }

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

        indents.push(newIndent);

        //return true;
    }else{
        indents.push(""); //add empty string to preserve balance with removing later
    }

    //return false;

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

void PlSqlFormatter::chopLastIndent()
{
    if(indents.size() > 0){
        indents.last().chop(1); //so that closing parenthesis is on same column as opening
    }
}

void PlSqlFormatter::addLineBreak()
{
    result.append('\n');
}

bool PlSqlFormatter::formatGenericConstruct()
{
    bool formatted = false;

    if(isBracket()){
        formatParameterList(getClosingBracket());
        formatted = true;
    }

    return formatted;
}

bool PlSqlFormatter::formatParameterList(int closingToken, int nestingLevel)
{
    formatDefaultToken();
    indentToEnd(result);
    bool indentedToEnd = true;

    int commaCount = 0;
    bool multiline = false;
    bool innerMultiline = false;

    int paramCountPerLine = getParamCountPerLine();

    do{
        readNextToken();

        if(isBracket()){
            innerMultiline = formatParameterList(getClosingBracket(), nestingLevel+1);
        }else if(token == closingToken){
            if(multiline || innerMultiline){
                addLineBreak();
                chopLastIndent();
                indent(result);
            }

            if(indentedToEnd){
                unindent();
            }

            formatDefaultToken();

            return multiline;
        }else{

            /*if(token == selectKeyword){
                formatSelectStatement(true);

                if(indentedToEnd){
                    unindent();
                }

                return multiline;
            }*/

            bool formatted = formatGenericConstruct();
            if(!formatted){
                formatDefaultToken();

                if(token == PLS_COMMA && currentScopeName()=="PARAMETER_LIST"){
                    ++commaCount;
                    if(commaCount >= paramCountPerLine){
                        addLineBreak();
                        multiline = true;
                        commaCount = 0;
                    }
                }
            }
        }


    }while(token != PLS_E_O_F && token != PLS_SEMI);

    return multiline;
}

void PlSqlFormatter::formatDefaultToken()
{
    if(currTokenInfo->tokenOrRuleId == PLS_E_O_F){
        return;
    }

    QString lexeme = currTokenInfo->lexeme;

    QList<CodeFormatterAction*> actions = settings.actionsForToken(token, lexeme);

    QList<CodeFormatterAction*> prevTokenActions;
    TokenInfo *prevTokenInfo = getPrevToken();
    if(prevTokenInfo != 0){
        prevTokenActions = settings.actionsForToken(prevTokenInfo->tokenOrRuleId, prevTokenInfo->lexeme);
    }

    executeActions(actions, CodeFormatterAction::Before, prevTokenInfo, prevTokenActions);

    QChar lastChar = result.length() > 0 ? result.right(1)[0] : QChar();

    if((currTokenInfo->tokenOrRuleId == PLS_SL_COMMENT ||
            currTokenInfo->tokenOrRuleId == PLS_ML_COMMENT) &&
            (prevTokenInfo!=0 && prevTokenInfo->endLine == currTokenInfo->startLine && lastChar == '\n')){

        result.insert(result.lastIndexOf('\n'), QString(" %1").arg(currTokenInfo->lexeme));
        //executeActions(actions, CodeFormatterAction::After, prevTokenInfo, prevTokenActions);
        return;
    }

    bool prependSpace = !lastChar.isNull() && !lastChar.isSpace() && lastChar != '\n' &&
                            !containsAction(actions, CodeFormatterAction::Before, CodeFormatterAction::NoSpace) &&
                            !containsAction(prevTokenActions, CodeFormatterAction::After, CodeFormatterAction::NoSpace);

    if(prependSpace){
        result.append(' ');
    }

    //restore empty lines entered by user when writing code
    TokenInfo *prevAnyTokenInfo = getPrevToken(false);
    if(prevAnyTokenInfo!=0){
        int lastTokenEndLine = prevAnyTokenInfo->endLine;
        int currTokenStartLine = currTokenInfo->startLine;

        int lineDiff = currTokenStartLine - lastTokenEndLine - 1;

        /*if(lastChar == '\n'){
            --lineDiff;
        }*/

        while(lineDiff-- > 0){
            addLineBreak();
            lastChar = '\n';
        }
    }

    if(lastChar == '\n'){
        indent(result);
    }

    result.append(lexeme);

    executeActions(actions, CodeFormatterAction::After, prevTokenInfo, prevTokenActions);
}

void PlSqlFormatter::executeActions(QList<CodeFormatterAction *> actions, CodeFormatterAction::ActionSequence sequence,
                                    TokenInfo *prevTokenInfo, QList<CodeFormatterAction *> prevTokenActions)
{
    foreach(CodeFormatterAction *action, actions){
        if(action->getSequence() == sequence){
            applyAction(action, prevTokenInfo, prevTokenActions);
        }
    }
}

void PlSqlFormatter::applyAction(CodeFormatterAction *action,
                                 TokenInfo *prevTokenInfo,
                                 const QList<CodeFormatterAction *> &prevTokenActions)
{

    QList<QChar> afterCharList = action->charList("after");
    QStringList afterStringList = action->stringList("after", 2);
    QString currentScope = currentScopeName();
    QStringList inScope = action->stringList("in_scope");
    QStringList notInScope = action->stringList("not_in_scope");

    bool okToApply = !endsWith(result, action->charList("not_after")) &&
                     (prevTokenInfo==0 || !action->stringList("not_after", 2).contains(prevTokenInfo->lexeme, Qt::CaseInsensitive)) &&
                     (afterCharList.isEmpty() || (prevTokenInfo!=0 && endsWith(result, afterCharList, true))) &&
                     (afterStringList.isEmpty() || (prevTokenInfo!=0 && afterStringList.contains(prevTokenInfo->lexeme, Qt::CaseInsensitive))) &&
                     (inScope.isEmpty() || (!currentScope.isEmpty() && inScope.contains(currentScope, Qt::CaseInsensitive))) &&
                     (notInScope.isEmpty() || !notInScope.contains(currentScope, Qt::CaseInsensitive));

    if(!okToApply){
        return;
    }

    switch(action->getType()){
    case CodeFormatterAction::Newline:
        if(action->getSequence() == CodeFormatterAction::Before){
            if(!result.isEmpty() &&
                    (action->boolValue("repeat")==true ||
                     !containsAction(prevTokenActions, CodeFormatterAction::Any, CodeFormatterAction::Newline)) &&
                    !endsWith(result, QList<QChar>() << '\n')){
                addLineBreak();
            }
        }else{
            addLineBreak();
        }
        break;
    case CodeFormatterAction::EmptyLine:
        if(action->getSequence() == CodeFormatterAction::Before && !result.isEmpty()){
            if(result.endsWith("\n\n")){
                //do nothing
            }else if(result.endsWith("\n")){
                addLineBreak();
            }else{
                addLineBreak();
                addLineBreak();
            }
        }else if(action->getSequence() == CodeFormatterAction::After){
            addLineBreak();
            addLineBreak();
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
    case CodeFormatterAction::IndentToEnd:
        indentToEnd(result, action->boolValue("additional_space"));
        break;
    case CodeFormatterAction::Unindent:
        unindent();
        break;
    case CodeFormatterAction::EnterScope:
    {
        QString newScopeName = action->string("scope_name");
        Q_ASSERT(newScopeName.length() > 0);
        QString maxParamCount = action->string("max_param_count");
        int paramCount = maxParamCount.isEmpty() ? DEFAULT_PARAM_COUNT_PER_LINE : maxParamCount.toInt();
        enterScope(newScopeName, paramCount);

        break;
    }
    case CodeFormatterAction::ExitScope:
    {
        QString scopeName = action->string("scope_name");
        Q_ASSERT(scopeName.length() > 0);
        exitScope(scopeName);
        break;
    }
    case CodeFormatterAction::ChangeScope:
    {
        QStringList fromList = action->stringList("from");
        QString to = action->string("to");

        Q_ASSERT(!fromList.isEmpty() && !to.isEmpty());

        if(fromList.contains(currentScopeName(), Qt::CaseInsensitive)){
            exitScope(currentScopeName());

            QString maxParamCount = action->string("max_param_count");
            int paramCount = maxParamCount.isEmpty() ? DEFAULT_PARAM_COUNT_PER_LINE : maxParamCount.toInt();

            enterScope(to, paramCount);
        }

        break;
    }
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

bool PlSqlFormatter::isBracket() const
{
    return token == PLS_LPAREN || token == PLS_LBRACK || token == PLS_LCURLY_BRACK;
}

int PlSqlFormatter::getClosingBracket() const
{
    int result = -1;

    switch(token){
    case PLS_LPAREN:
        result = PLS_RPAREN;
        break;
    case PLS_LBRACK:
        result = PLS_RBRACK;
        break;
    case PLS_LCURLY_BRACK:
        result = PLS_RCURLY_BRACK;
        break;
    default:
        Q_ASSERT(false);
        break;
    }

    return result;
}

bool PlSqlFormatter::isWhitespace() const
{
    return token == PLS_SL_COMMENT || token == PLS_ML_COMMENT;
}

ScopeInfo PlSqlFormatter::currentScope() const
{
    if(scope.isEmpty()){
        return ScopeInfo();
    }

    return scope.last();
}

QString PlSqlFormatter::currentScopeName() const
{
    return currentScope().scopeName;
}

void PlSqlFormatter::enterScope(const QString &scopeName, int maxParamCount)
{
    scope.append(ScopeInfo(scopeName, maxParamCount));
}

void PlSqlFormatter::exitScope(const QString &scopeName)
{
    if(scope.isEmpty()){
        return;
    }

    if(!scopeName.isEmpty() && currentScopeName() != scopeName){
        return;
    }

    scope.removeLast();
}

int PlSqlFormatter::getParamCountPerLine() const
{
    //last scope must be PARAMETER_LIST
    if(scope.size() < 2){
        return DEFAULT_PARAM_COUNT_PER_LINE;
    }else{
        return scope[scope.size()-2].maxParamCount;
    }
}
