#include "textcursorreader.h"

TextCursorReader::TextCursorReader(const QTextCursor &cursor) : cursor(cursor)
{
}

QChar TextCursorReader::getNextCharacter()
{
    Q_ASSERT(!isEOF());

    cursor.clearSelection();
    cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
    QString str=cursor.selectedText();
    Q_ASSERT(str.size()==1);

    return str.at(0);
}

void TextCursorReader::ungetCharacter()
{
    Q_ASSERT(!cursor.atStart());

    cursor.movePosition(QTextCursor::PreviousCharacter);
}

bool TextCursorReader::isEOF() const
{
    return cursor.atEnd();
}
