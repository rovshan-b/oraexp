#ifndef ITEXTREADER_H
#define ITEXTREADER_H

#include <QChar>

class ITextReader
{
public:
    virtual QChar getNextCharacter()=0;

    virtual void ungetCharacter()=0;

    virtual bool isEOF() const=0;
};

#endif // ITEXTREADER_H
