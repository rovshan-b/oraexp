#include "stringreader.h"

StringReader::StringReader(const QString &str) : str(str), lastSearchPos(0), reachedEOF(false)
{
}

bool StringReader::isEOF() const
{
    return str.size()==0 || reachedEOF;
}

bool StringReader::atStart() const
{
    return false;
}

QString StringReader::getNextLine()
{
    Q_ASSERT(!isEOF());

    int newlineIx=str.indexOf('\n', lastSearchPos);
    if(newlineIx==-1){
        reachedEOF=true;
        return str.mid(lastSearchPos).append("\n");
    }else{
        QString newLine=str.mid(lastSearchPos, newlineIx+1-lastSearchPos);
        lastSearchPos = newlineIx + 1;

        return newLine;
    }
}
