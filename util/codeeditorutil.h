#ifndef CODEEDITORUTIL_H
#define CODEEDITORUTIL_H

#include "codeeditor/codeeditor.h"
#include "connectivity/ociexception.h"
#include "beans/tokennameinfo.h"

class ParseTreeNode;
class TokenInfo;

class CodeEditorUtil
{
public:
    static void highlightEditorError(CodeEditor *editor, int errorPos, const OciException &ex, bool append=false);
    static void highlightEditorError(CodeEditor *editor, int errorPos, int errorCode, const QString &errorMessage, bool append=false);

    static void markPosition(CodeEditor *editor, int pos);

    static QString getCurrentObjectName(CodeEditor *editor);
    static TokenNameInfo getCurrentObjectNameInfo(CodeEditor *editor);
    static TokenNameInfo getObjectNameInfo(const QTextCursor &cur, bool upToCurrent = true);
    static TokenNameInfo getObjectNameInfoFromSelection(const QTextCursor &cur);

    static QList<ParseTreeNode*> getDeclarationsForCurrentToken(const CodeEditor *editor,
                                                                const QTextCursor &cur,
                                                                bool *foundInPairEditor,
                                                                TokenInfo **currTokenInfo,
                                                                ParseTreeNode **discardReason);

    static void formatCode(CodeEditor *editor);

private:
    CodeEditorUtil();

    static bool isIdentifierOrSeparatorToken(int token);
};

#endif // CODEEDITORUTIL_H
