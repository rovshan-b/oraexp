#ifndef CODEEDITORUTIL_H
#define CODEEDITORUTIL_H

#include "codeeditor/codeeditor.h"
#include "connectivity/ociexception.h"

class CodeEditorUtil
{
public:
    static void highlightEditorError(CodeEditor *editor, int errorPos, const OciException &ex, bool append=false);
    static void highlightEditorError(CodeEditor *editor, int errorPos, int errorCode, const QString &errorMessage, bool append=false);

    static void markPosition(CodeEditor *editor, int pos);

    static QString getCurrentObjectName(CodeEditor *editor);

private:
    CodeEditorUtil();

    static bool isWordChar(const QChar &c);
};

#endif // CODEEDITORUTIL_H
