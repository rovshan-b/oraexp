#ifndef TEXTCURSORREADER_H
#define TEXTCURSORREADER_H

#include "textreaderbase.h"
#include <QTextCursor>

class TextCursorReader : public TextReaderBase
{
public:
    TextCursorReader(const QTextCursor &cursor);

    virtual bool isEOF() const;
    virtual bool atStart() const;

protected:
    virtual QString getNextLine();

private:
    QTextCursor cursor;
    bool started;
};

#endif // TEXTCURSORREADER_H
