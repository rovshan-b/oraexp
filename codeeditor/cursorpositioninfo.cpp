#include "cursorpositioninfo.h"

CursorPositionInfo::CursorPositionInfo()
{
}

void CursorPositionInfo::selectText(QTextCursor &cur)
{
    if(anchorAtEnd){
        cur.setPosition(endPos);
        cur.setPosition(startPos, QTextCursor::KeepAnchor);
    }else{
        cur.setPosition(startPos);
        cur.setPosition(endPos, QTextCursor::KeepAnchor);
    }
}
