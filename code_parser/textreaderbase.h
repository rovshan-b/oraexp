#ifndef TEXTREADERBASE_H
#define TEXTREADERBASE_H

#include <QString>

class TextReaderBase
{
public:
    explicit TextReaderBase();
    virtual ~TextReaderBase(){}

    QChar getNextCharacter();

    void ungetCharacter();

    int getCurrPos() const {return this->pos;}
    int getCurrLineNo() const {return this->lineNo;}
    int getCurrLinePos() const {return this->linePos;}

    QString getCurrentLine() const {return this->buffer;}

protected:
    virtual QString getNextLine()=0;

    //below functions must be implemented in child classes
    //and are only for use by this base class for determining
    //if it can get some text for filling its buffer
    virtual bool isEOF() const=0;
    virtual bool atStart() const=0;
private:
    QString buffer;

    int pos;
    int lineNo;
    int linePos;

    //int lineCountInBuffer;
    int bufferPos;

    bool readIntoBuffer();
};

#endif // TEXTREADERBASE_H
