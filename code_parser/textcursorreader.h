#ifndef TEXTCURSORREADER_H
#define TEXTCURSORREADER_H

#include "itextreader.h"
#include <QTextCursor>

class TextCursorReader : public ITextReader
{
public:
    TextCursorReader(const QTextCursor &cursor);

    virtual QChar getNextCharacter();
    virtual void ungetCharacter();
    virtual bool isEOF() const;

private:
    QTextCursor cursor;
};

#endif // TEXTCURSORREADER_H
