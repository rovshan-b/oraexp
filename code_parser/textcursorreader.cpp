#include "textcursorreader.h"

TextCursorReader::TextCursorReader(const QTextCursor &cursor) : cursor(cursor), started(false)
{
}

QString TextCursorReader::getNextLine()
{
    Q_ASSERT(!isEOF());

    if(started){
        cursor.movePosition(QTextCursor::NextBlock);
    }else{
        //cursor.movePosition(QTextCursor::StartOfLine);
        started=true;
    }
    cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);

    QString line = cursor.selectedText();
    line.append("\n");

    return line;
}

bool TextCursorReader::isEOF() const
{
    return cursor.atEnd();
}

bool TextCursorReader::atStart() const
{
    return cursor.atStart();
}
