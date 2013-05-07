#include "textstreamreader.h"

TextStreamReader::TextStreamReader(QTextStream *textStream) : textStream(textStream)
{
}

bool TextStreamReader::isEOF() const
{
    return textStream->status() == QTextStream::ReadPastEnd;
}

bool TextStreamReader::atStart() const
{
    return false;
}

bool TextStreamReader::resetPosition()
{
    return textStream->seek(0);
}

QString TextStreamReader::getNextLine()
{
    Q_ASSERT(!isEOF());

    return textStream->readLine(50000);
}
