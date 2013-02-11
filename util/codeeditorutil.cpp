#include "codeeditorutil.h"
#include "errors.h"
#include "code_parser/plsql/plsqlscanner.h"
#include "code_parser/textcursorreader.h"
#include "code_parser/stringreader.h"
#include "code_parser/plsql/plsqltokens.h"

CodeEditorUtil::CodeEditorUtil()
{
}

void CodeEditorUtil::highlightEditorError(CodeEditor *editor, int errorPos, const OciException &ex)
{
    QTextCursor errorPositionCursor = editor->textCursor();
    errorPositionCursor.setPosition(errorPos); //errorPos is 1 based

    int errorCode = ex.getErrorCode();
    QString errMsg = ex.getErrorMessage();

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
             errorCode == ERR_COMPONENT_MUST_BE_DECALRED){
        QTextCursor editorReaderCursor = errorPositionCursor;
        QScopedPointer<PlSqlScanner> editorScanner(new PlSqlScanner(new TextCursorReader(editorReaderCursor)));
        editorScanner->getNextToken(); //move one token forward
        int endPos=editorScanner->getCurrPos();
        errorPositionCursor.setPosition(errorPositionCursor.position()+endPos, QTextCursor::KeepAnchor);
        marked = true;
    }

    if(!marked){
        errorPositionCursor.movePosition(!errorPositionCursor.atEnd() ?
                                             QTextCursor::NextCharacter
                                           :
                                             QTextCursor::PreviousCharacter,
                                         QTextCursor::KeepAnchor);
    }

    editor->setErrorPosition(errorPositionCursor);
}
