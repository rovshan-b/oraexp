#ifndef TEXTSTREAMREADER_H
#define TEXTSTREAMREADER_H

#include "textreaderbase.h"

#include <QTextStream>

class TextStreamReader : public TextReaderBase
{
public:
    TextStreamReader(QTextStream *textStream);

    virtual bool isEOF() const;
    virtual bool atStart() const;

    bool resetPosition();

protected:
    virtual QString getNextLine();

private:
    QTextStream *textStream;
};

#endif // TEXTSTREAMREADER_H
