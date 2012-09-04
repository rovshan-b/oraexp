#ifndef CURSORPOSITIONINFO_H
#define CURSORPOSITIONINFO_H

#include <QTextCursor>

class CursorPositionInfo
{
public:
    CursorPositionInfo();

    void selectText(QTextCursor &cur);

    int startPos;
    int endPos;
    int startBlock;
    int endBlock;

    bool anchorAtEnd;
};

#endif // CURSORPOSITIONINFO_H
