#include "codeeditorutil.h"
#include "errors.h"
#include "code_parser/plsql/plsqlscanner.h"
#include "code_parser/textcursorreader.h"
#include "code_parser/stringreader.h"
#include "code_parser/plsql/plsqltokens.h"
#include "code_parser/plsql/plsqlparsehelper.h"
#include "code_parser/plsql/plsqltreebuilder.h"
#include "code_formatter/plsql/plsqlformatter.h"
#include "codeeditor/blockdata.h"
#include "beans/tokeninfo.h"
#include "util/strutil.h"
#include <QTextBlock>

CodeEditorUtil::CodeEditorUtil()
{
}

void CodeEditorUtil::highlightEditorError(CodeEditor *editor, int errorPos, const OciException &ex, bool append)
{
    int errorCode = ex.getErrorCode();
    QString errorMessage = ex.getErrorMessage();

    CodeEditorUtil::highlightEditorError(editor, errorPos, errorCode,  errorMessage, append);
}

void CodeEditorUtil::highlightEditorError(CodeEditor *editor, int errorPos, int errorCode, const QString &errorMessage, bool append)
{
    QTextCursor errorPositionCursor = editor->textCursor();
    errorPositionCursor.setPosition(errorPos); //errorPos is 1 based

    QString errMsg = errorMessage;

    if(errorCode==ERR_PLS_ERROR_POSITION){ //second line contains real error description
        errMsg = errMsg.mid(errMsg.indexOf('\n')+1); //removed first line
        errMsg = errMsg.mid(0, errMsg.indexOf('\n')); //keeping only second line
        errorCode = PlSqlParseHelper::extractPlSqlErrorCode(errMsg);
    }

    bool marked = false;
    if(errorCode == ERR_INVALID_IDENTIFIER){
        int firstColonIx = errMsg.indexOf(':');
        int secondColonIx = errMsg.lastIndexOf(':');

        if(firstColonIx!=-1 && secondColonIx!=-1){
            QString identifierName = errMsg.mid(firstColonIx+1, secondColonIx-firstColonIx-1).trimmed();
            if(!identifierName.isEmpty()){
                //errorPositionCursor.movePosition(QTextCursor::PreviousCharacter);
                QTextCursor editorReaderCursor = errorPositionCursor;
                QScopedPointer<PlSqlScanner> editorScanner(new PlSqlScanner(new TextCursorReader(editorReaderCursor)));
                QScopedPointer<PlSqlScanner> identifierScanner(new PlSqlScanner(new StringReader(identifierName)));

                int token;
                while(true){
                    token = identifierScanner->getNextToken();

                    if(token!=PLS_E_O_F){
                        editorScanner->getNextToken();
                    }else{
                        break;
                    }
                }

                int endPos=editorScanner->getCurrPos();
                errorPositionCursor.setPosition(errorPositionCursor.position()+endPos, QTextCursor::KeepAnchor);
                marked = true;
            }
        }
    }else if(errorCode == ERR_TABLE_OR_VIEW_DOES_NOT_EXIST ||
                errorCode == ERR_COMPONENT_MUST_BE_DECALRED ||
                errorCode == ERR_IDENTIFIER_MUST_BE_DECLARED ||
                errorCode == ERR_AMBIGUOUS_COLUMN_NAME ||
                errorCode == ERR_NAME_ALREADY_USED ||
                errorCode == ERR_MUST_BE_DECLARED){
        QTextCursor editorReaderCursor = errorPositionCursor;
        QScopedPointer<PlSqlScanner> editorScanner(new PlSqlScanner(new TextCursorReader(editorReaderCursor)));
        int token=editorScanner->getNextToken(); //move one token forward
        if(token!=PLS_E_O_F && token!=PLS_ERROR){
            int endPos=editorScanner->getCurrPos();
            int newPos=errorPositionCursor.position()+endPos;
            errorPositionCursor.setPosition(newPos, QTextCursor::KeepAnchor);
            if(errorPositionCursor.position()==newPos){
                marked = true;
            }
        }
    }

    if(!marked){
        errorPositionCursor.movePosition(!errorPositionCursor.atEnd() ?
                                             QTextCursor::NextCharacter
                                           :
                                             QTextCursor::PreviousCharacter,
                                         QTextCursor::KeepAnchor);
    }

    append ? editor->addErrorPosition(errorPositionCursor) : editor->setErrorPosition(errorPositionCursor);
}

void CodeEditorUtil::markPosition(CodeEditor *editor, int pos)
{
    QTextCursor cur = editor->textCursor();
    cur.setPosition(pos);
    editor->setMarkedLine(cur.blockNumber());
    editor->setTextCursor(cur);
    editor->centerCursor();
}

QString CodeEditorUtil::getCurrentObjectName(CodeEditor *editor)
{
    return CodeEditorUtil::getCurrentObjectNameInfo(editor).toString();
}

TokenNameInfo CodeEditorUtil::getCurrentObjectNameInfo(CodeEditor *editor)
{
    QTextCursor cur = editor->textCursor();
    if(cur.hasSelection()){
        return CodeEditorUtil::getObjectNameInfoFromSelection(cur);
    }else{
        return CodeEditorUtil::getObjectNameInfo(cur);
    }
}

TokenNameInfo CodeEditorUtil::getObjectNameInfo(const QTextCursor &cur, bool upToCurrent)
{
    TokenNameInfo result;

    QTextBlock block = cur.block();
    if(!block.isValid()){
        return result;
    }

    BlockData *data = static_cast<BlockData*>(block.userData());
    Q_ASSERT(data);

    TokenInfo *currToken = data->tokenAtPosition(cur.positionInBlock());
    if(!currToken){
        return result;
    }

    if(!PlSqlParseHelper::isIdentifierToken(currToken->tokenOrRuleId)){
        return result;
    }

    QList<TokenInfo*> blockTokens = data->getTokens();

    int currTokenIx = blockTokens.indexOf(currToken);
    Q_ASSERT(currTokenIx != -1);

    TokenInfo *siblingToken;

    QTextCursor curCopy = cur;

    int tokenIx = currTokenIx;
    QTextBlock activeBlock = block;

    int lastTokenType = -1;

    //scan to left
    do{

        if(tokenIx < 0){
            activeBlock = activeBlock.previous();
            if(!activeBlock.isValid()){
                break;
            }
            data = static_cast<BlockData*>(activeBlock.userData());
            Q_ASSERT(data);
            blockTokens = data->getTokens();
            if(blockTokens.size() == 0){
                continue;
            }
            tokenIx = blockTokens.size()-1;
        }

        siblingToken = blockTokens.at(tokenIx--);

        if((PlSqlParseHelper::isIdentifierToken(siblingToken->tokenOrRuleId) && PlSqlParseHelper::isIdentifierToken(lastTokenType)) ||
                (PlSqlParseHelper::isIdentifierSeparatorToken(siblingToken->tokenOrRuleId) && PlSqlParseHelper::isIdentifierSeparatorToken(lastTokenType))){
            break;
        }

        if(!PlSqlParseHelper::isIdentifierOrSeparatorToken(siblingToken->tokenOrRuleId)){
            break;
        }

        if(!PlSqlParseHelper::isIdentifierSeparatorToken(siblingToken->tokenOrRuleId)){ //if it is not PLS_DOT then it is either PLS_ID or PLS_DOUBLEQUOTED_ID
            curCopy.setPosition(activeBlock.position() + siblingToken->startPos);
            curCopy.setPosition(activeBlock.position() + siblingToken->endPos, QTextCursor::KeepAnchor);

            QString lexeme = curCopy.selectedText();

            result.parts.prepend(PlSqlParseHelper::cleanIdentifier(lexeme));
            result.absolutePositions.prepend(qMakePair(curCopy.selectionStart(), curCopy.selectionEnd()));
        }

        lastTokenType = siblingToken->tokenOrRuleId;

    }while(PlSqlParseHelper::isIdentifierOrSeparatorToken(siblingToken->tokenOrRuleId));

    result.currentPartId = result.parts.size() - 1;

    data = static_cast<BlockData*>(block.userData());
    blockTokens = data->getTokens();
    activeBlock = block;

    //scan to right

    tokenIx = currTokenIx + 1;

    lastTokenType = currToken->tokenOrRuleId;

    if(!upToCurrent){ //do not scan to right if we want to get name up to current token

        do{

            if(tokenIx == blockTokens.size()){
                while(true){
                    activeBlock = activeBlock.next();
                    if(!activeBlock.isValid()){
                        return result;
                    }
                    data = static_cast<BlockData*>(activeBlock.userData());
                    Q_ASSERT(data);
                    blockTokens = data->getTokens();
                    if(blockTokens.size() == 0){
                        continue;
                    }
                    tokenIx = 0;
                    break;
                }
            }

            siblingToken = blockTokens.at(tokenIx++);

            if((PlSqlParseHelper::isIdentifierToken(siblingToken->tokenOrRuleId) && PlSqlParseHelper::isIdentifierToken(lastTokenType)) ||
                    (PlSqlParseHelper::isIdentifierSeparatorToken(siblingToken->tokenOrRuleId) && PlSqlParseHelper::isIdentifierSeparatorToken(lastTokenType))){
                break;
            }

            if(!PlSqlParseHelper::isIdentifierOrSeparatorToken(siblingToken->tokenOrRuleId)){
                break;
            }

            if(!PlSqlParseHelper::isIdentifierSeparatorToken(siblingToken->tokenOrRuleId)){ //if it is not PLS_DOT then it is either PLS_ID or PLS_DOUBLEQUOTED_ID
                curCopy.setPosition(activeBlock.position() + siblingToken->startPos);
                curCopy.setPosition(activeBlock.position() + siblingToken->endPos, QTextCursor::KeepAnchor);

                QString lexeme = curCopy.selectedText();

                result.parts.append(PlSqlParseHelper::cleanIdentifier(lexeme));
                result.absolutePositions.append(qMakePair(curCopy.selectionStart(), curCopy.selectionEnd()));
            }

            lastTokenType = siblingToken->tokenOrRuleId;

        }while(PlSqlParseHelper::isIdentifierOrSeparatorToken(siblingToken->tokenOrRuleId));

    }

    return result;
}

TokenNameInfo CodeEditorUtil::getObjectNameInfoFromSelection(const QTextCursor &cur)
{
    Q_ASSERT(cur.hasSelection());

    TokenNameInfo result;

    QString selectedText = cur.selectedText();

    if(selectedText.trimmed().length() > 250){
        return result;
    }

    int curPos = qMin(cur.selectionStart(), cur.selectionEnd());


    QScopedPointer<PlSqlScanner> scanner(new PlSqlScanner(new StringReader(selectedText)));

    int token;
    int lastTokenType = -1;

    do{
        token = scanner->getNextToken();

        if(token == lastTokenType){
            break;
        }

        if(PlSqlParseHelper::isIdentifierToken(token)){
            result.parts.append(PlSqlParseHelper::cleanIdentifier(scanner->getTokenLexeme()));
            result.absolutePositions.append(qMakePair(curPos + scanner->getTokenStartPos(), curPos + scanner->getTokenEndPos()));
        }

        lastTokenType = token;

    }while(PlSqlParseHelper::isIdentifierOrSeparatorToken(token));

    result.currentPartId = result.parts.count() - 1;

    return result;
}

QList<ParseTreeNode *> CodeEditorUtil::getDeclarationsForCurrentToken(const CodeEditor *editor,
                                                                      const QTextCursor &cur,
                                                                      bool *foundInPairEditor,
                                                                      TokenInfo **currTokenInfo,
                                                                      ParseTreeNode **discardReason)
{
    *discardReason = 0;

    QList<ParseTreeNode*> nodeList;

    if(editor->getLastParseId() == -1){
        return nodeList;
    }

    PlSqlTreeBuilder *treeBuilder = editor->getTreeBuilder();

    Q_ASSERT(treeBuilder);

    QTextBlock block = cur.block();
    BlockData *data = static_cast<BlockData*>(block.userData());
    if(!data){
        return nodeList;
    }

    TokenInfo *tokenInfo = data->tokenAtPosition(cur.positionInBlock());
    if(!tokenInfo || !PlSqlParseHelper::isIdentifierToken(tokenInfo->tokenOrRuleId)){
        return nodeList;
    }

    *currTokenInfo = tokenInfo;

    nodeList = treeBuilder->findDeclarations(block.position() + tokenInfo->startPos, discardReason);

    if(*discardReason){
        return nodeList;
    }

    CodeEditor *pairEditor = editor->getPairEditor();

    if(nodeList.isEmpty() && pairEditor!=0 && pairEditor->getLastParseId() != -1){
        QTextCursor copyCur = cur;

        copyCur.setPosition(block.position() + tokenInfo->startPos);
        copyCur.setPosition(block.position() + tokenInfo->endPos, QTextCursor::KeepAnchor);

        QString lexeme = copyCur.selectedText();

        nodeList = pairEditor->getTreeBuilder()->findDeclarations(lexeme);

        *foundInPairEditor = true;
    }else{
        *foundInPairEditor = false;
    }

    return nodeList;

}

void CodeEditorUtil::formatCode(CodeEditor *editor)
{
    QTextCursor cur = editor->textCursor();
    int cursorPosition = cur.position();

    cur.beginEditBlock();

    QString code = editor->toPlainText();

    CursorPositionInfo cursorInfo;
    QString indent;

    bool allText = false;

    if(!cur.hasSelection()){
        allText = true;
        cur.setPosition(0);
        cur.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
        code = editor->toPlainText();
    }else{
        cursorInfo = editor->getStartStopPositions(cur);
        code = replaceParagraphSeparators(cur.selectedText());
        QTextBlock block = editor->document()->findBlockByNumber(cursorInfo.startBlock);
        QString blockText = block.text();
        for(int i=0; i<cursorInfo.startPos-block.position(); ++i){
            if(blockText.at(i).isSpace()){
                indent.append(blockText.at(i)); //may be a tab character
            }else{
                indent.append(' ');
            }
        }
    }

    QString result;

    if(!editor->isPlsqlMode()){ //can contain many pieces of different statements
        int subqueryStartPos=0, subqueryEndPos=0;
        int prevEndPosition = -1;
        int currentPos;

        while(subqueryStartPos!=-1 && subqueryEndPos!=-1){
            currentPos = subqueryEndPos;
            PlSqlParseHelper::getNextQueryPos(code, currentPos, &subqueryStartPos, &subqueryEndPos);

            if(subqueryStartPos!=-1 && subqueryEndPos!=-1){
                QString subquery = code.mid(subqueryStartPos, subqueryEndPos-subqueryStartPos-1);

                if(prevEndPosition!=-1){
                    result.append(code.mid(prevEndPosition-1, subqueryStartPos-prevEndPosition+1));
                }

                result.append(PlSqlFormatter().format(subquery));

                prevEndPosition = subqueryEndPos;
            }
        }

        if(prevEndPosition!=-1){
            result.append(code.mid(prevEndPosition-1, subqueryStartPos-prevEndPosition+1));
        }
    }else{ //format as single statement
        result.append(PlSqlFormatter().format(code));
    }

    if(!allText){
        result.replace("\n", QString("\n%1").arg(indent));
    }

    cur.insertText(result.trimmed());

    cur.setPosition(cursorPosition);
    cur.endEditBlock();

    editor->setTextCursor(cur);
}
