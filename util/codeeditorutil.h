#ifndef CODEEDITORUTIL_H
#define CODEEDITORUTIL_H

#include "codeeditor/codeeditor.h"
#include "connectivity/ociexception.h"

class CodeEditorUtil
{
public:
    static void highlightEditorError(CodeEditor *editor, int errorPos, const OciException &ex, bool append=false);

private:
    CodeEditorUtil();
};

#endif // CODEEDITORUTIL_H
