#include "textreaderbase.h"

TextReaderBase::TextReaderBase() : pos(0), lineNo(0), linePos(0), lineCountInBuffer(0), bufferPos(0)
{
}

QChar TextReaderBase::getNextCharacter()
{
    bool couldRead=true;
    if(buffer.isEmpty()){
        couldRead=readIntoBuffer();
    }else if(bufferPos==buffer.size()-1){
        couldRead=readIntoBuffer();
        if(couldRead){
            ++lineNo;
        }
    }

    if(!couldRead){
        return QChar();
    }

    QChar nextChar=buffer.at(bufferPos);
    ++bufferPos;
    ++pos;

    return nextChar;
}

void TextReaderBase::ungetCharacter()
{
    Q_ASSERT(!atStart());
    Q_ASSERT(bufferPos>0);

    --bufferPos;
    --pos;

    if(buffer.at(bufferPos)=='\n'){
        --lineNo;
        int lastNewlineIx=buffer.lastIndexOf('\n', bufferPos-1);
        if(lastNewlineIx==-1){
            linePos=buffer.size()-2;
        }else{
            linePos=buffer.mid(lastNewlineIx+1).size()-1;
        }
    }
}

bool TextReaderBase::readIntoBuffer()
{
    if(isEOF()){
        return false;
    }

    QString line = getNextLine();
    linePos=0;

    buffer.append(line);
    ++lineCountInBuffer;

    return true;
}
