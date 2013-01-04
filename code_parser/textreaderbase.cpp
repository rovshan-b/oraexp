#include "textreaderbase.h"

TextReaderBase::TextReaderBase() : pos(0), lineNo(0), linePos(0), /*lineCountInBuffer(0),*/ bufferPos(0)
{
}

QChar TextReaderBase::getNextCharacter()
{
    bool couldRead=true;
    if(buffer.isEmpty()){
        couldRead=readIntoBuffer();
    }else if(bufferPos==buffer.size()){
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
    ++linePos;

    return nextChar;
}

void TextReaderBase::ungetCharacter()
{
    Q_ASSERT(!atStart());
    Q_ASSERT(bufferPos>0);

    --bufferPos;
    --pos;
    --linePos;

    /*
    if(bufferPos-2>0 && buffer.at(bufferPos-2)=='\n'){
        --lineNo;
        int lastNewlineIx=buffer.lastIndexOf('\n', bufferPos-1);
        if(lastNewlineIx==-1){
            linePos=buffer.size()-2;
        }else{
            linePos=buffer.mid(lastNewlineIx+1).size()-1;
        }
    }*/
}

bool TextReaderBase::readIntoBuffer()
{
    if(isEOF()){
        return false;
    }

    QString line = getNextLine();
    linePos=0;


    buffer=line;
    bufferPos=0;
    /*
    buffer.append(line);
    ++lineCountInBuffer;

    if(lineCountInBuffer>5){
        int newLineIx=buffer.indexOf('\n');
        buffer.remove(0, newLineIx+1);
        bufferPos-=newLineIx+1;

        --lineCountInBuffer;
    }*/

    return true;
}
