#ifndef STRINGREADER_H
#define STRINGREADER_H

#include "textreaderbase.h"

class StringReader : public TextReaderBase
{
public:
    StringReader(const QString &str, int startPos = 0);

    virtual bool isEOF() const;
    virtual bool atStart() const;

protected:
    virtual QString getNextLine();

private:
    QString str;

    int lastSearchPos;
    bool reachedEOF;
};

#endif // STRINGREADER_H
