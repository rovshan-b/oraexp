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

    static QList<ParseTreeNode*> getDeclarationsForLocalObject(CodeEditor *editor, const QTextCursor &cursor, bool *foundInPairEditor, ParseTreeNode **discardReason);

    static QList<ParseTreeNode*> getDeclarationsForCurrentToken(const CodeEditor *editor,
                                                                const QTextCursor &cur,
                                                                bool *foundInPairEditor,
                                                                ParseTreeNode **discardReason);

    static QList<ParseTreeNode*> getObjectLevelDeclarationsForLexeme(const CodeEditor *editor,
                                                          const QString &lexeme,
                                                          bool *foundInPairEditor);

    static ParseTreeNode *getPairDeclaration(CodeEditor *editor,
                                             const QTextCursor &cur,
                                             ParseTreeNode *procNode,
                                             const QList<ParseTreeNode *> &declList, bool *foundInPairEditor);

    static ParseTreeNode *getBestPairDeclarationByParamList(const ParseTreeNode *paramsNode,
                                                            const QList<ParseTreeNode*> &declList,
                                                            int pairRuleId);

    static void matchParameterLists(const ParseTreeNode *firstNode, const ParseTreeNode *secondNode,
                                    int *matchedParamCount, int *matchedParamNames);


    static void formatCode(CodeEditor *editor);

private:
    CodeEditorUtil();

    static bool isIdentifierOrSeparatorToken(int token);
};

#endif // CODEEDITORUTIL_H
