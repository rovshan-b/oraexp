#include "codeeditorutil.h"
#include "errors.h"
#include "code_parser/plsql/plsqlscanner.h"
#include "code_parser/textcursorreader.h"
#include "code_parser/stringreader.h"
#include "code_parser/plsql/plsqltokens.h"

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
        QString prefixToRemove = "PL/SQL:";
        if(errMsg.startsWith(prefixToRemove)){
            errMsg.remove(0, prefixToRemove.size());
        }
        //now find first colon
        int colonIx = errMsg.indexOf(':');
        if(colonIx!=-1){
            QString descriptiveErrorCode=errMsg.mid(0, colonIx); //for example ORA-00942
            int dashIx=descriptiveErrorCode.indexOf('-');
            if(dashIx!=-1){
                QString strErrorCode=descriptiveErrorCode.mid(dashIx+1); //keeping only numeric part: 00942
                bool canParseNumber;
                int tmpErrCode=strErrorCode.toInt(&canParseNumber);
                if(canParseNumber){
                    errorCode=tmpErrCode;
                }
            }
        }
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
                errorCode == ERR_NAME_ALREADY_USED){
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
    QTextCursor cur = editor->textCursor();

    if(cur.hasSelection()){
        return cur.selectedText();
    }

    int cursorPos = cur.position();
    QTextDocument *doc = editor->document();
    QChar c = doc->characterAt(cursorPos);

    while(c.isSpace() && cursorPos>0){
        c = doc->characterAt(--cursorPos);
    }

    if(cursorPos<=0){
        return "";
    }

    cur.setPosition(cursorPos);

    bool isWordChar = CodeEditorUtil::isWordChar(c);

    if(!isWordChar){
        return "";
    }

    //first move left
    QString fullWord;
    while(isWordChar){
        fullWord.prepend(c);

        c = doc->characterAt(--cursorPos);
        isWordChar = CodeEditorUtil::isWordChar(c);
    }

    cursorPos = cur.position();

    //now move right
    c = doc->characterAt(++cursorPos);
    isWordChar = CodeEditorUtil::isWordChar(c);

    while(isWordChar){
        fullWord.append(c);

        c = doc->characterAt(++cursorPos);
        isWordChar = CodeEditorUtil::isWordChar(c);
    }

    return fullWord;
}

bool CodeEditorUtil::isWordChar(const QChar &c)
{
    return PlSqlScanner::isIdCharacter(c) || c=='.' || c=='"';
}
