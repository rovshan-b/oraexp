#include "codecollapseposition.h"

CodeCollapsePosition::CodeCollapsePosition() :
    startLine(-1),
    endLine(-1)
{
}

CodeCollapsePosition::CodeCollapsePosition(int startLine, int endLine) :
    startLine(startLine),
    endLine(endLine)
{
}
