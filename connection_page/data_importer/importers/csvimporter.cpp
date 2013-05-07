#include "csvimporter.h"
#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>

CsvImporter::CsvImporter() :
    encoding(QObject::tr("System")),
    skipRows(0),
    headerOption(NoHeader),
    file(0),
    textStream(0)
{
}

CsvImporter::~CsvImporter()
{
    cleanup();
}

void CsvImporter::cleanup()
{
    buffer.clear();

    if(textStream){
        delete textStream;
        textStream = 0;
    }

    if(file){
        delete file;
        file = 0;
    }
}

QString CsvImporter::readString(int count)
{
    if(buffer.isEmpty()){
        return textStream->read(count);
    }else{
        QString result;
        if(buffer.size()>=count){
            result = buffer.left(count);
            buffer.remove(0, count);
            return result;
        }else{
            result.append(buffer);
            result.append(textStream->read(count - buffer.size()));

            return result;
        }
    }
}

QChar CsvImporter::readChar()
{
    QChar c;

    if(buffer.isEmpty()){
        *textStream >> c;
    }else{
        c = buffer[0];
        buffer.remove(0, 1);
    }

    return c;
}


bool CsvImporter::setFilename(const QString &filename)
{
    if(filename == this->filename){
        return true;
    }

    this->filename = filename;

    cleanup();

    file = new QFile(this->filename);
    if(!file->open(QIODevice::ReadOnly | QIODevice::Text)){
        cleanup();
        return false;
    }
    textStream = new QTextStream(file);
    if(!this->encoding.isEmpty()){
        textStream->setCodec(this->encoding.toStdString().c_str());
    }

    return true;
}

QStringList CsvImporter::readValues()
{
    QStringList values;

    maxEnclosureLength = 0;
    for(int i=0; i<enclosures.size(); ++i){
        maxEnclosureLength = qMax(maxEnclosureLength, enclosures.at(i).size());
    }

    bool eol = false;
    QString field;
    while(!eol){
        field = readNextField(&eol);

        if(isEOF()){
            if(!field.isEmpty()){
                values.append(field);
            }
            break;
        }else{
            values.append(field);
        }

        if(values.size()>1500){ //in case user by accident selected large binary file
            break;
        }
    }

    return values;
}

QString CsvImporter::readNextField(bool *readEndOfLine)
{
    *readEndOfLine = false;

    Q_ASSERT(!isEOF());

    QString field;
    QString currentFieldEnclosure;

    if(field.size() < maxEnclosureLength){
        int countToRead = maxEnclosureLength-field.size();
        for(int i=0; i<countToRead; ++i){
            QChar c = readChar();
            if(c=='\n' || c.isNull()){
                *readEndOfLine=true;
                return field;
            }else{
                field.append(c);
            }
        }
    }

    for(int k=0; k<enclosures.size(); ++k){
        QString enclosure = enclosures.at(k);
        if(field.startsWith(enclosure)){
            currentFieldEnclosure = enclosure;
            field.clear();
            break;
        }
    }

    QChar c;
    bool stopOnDelimiter = currentFieldEnclosure.isEmpty();
    bool stop = false;

    if(currentFieldEnclosure.isEmpty() && field==delimiter){
        return "";
    }

    do{
        c = readChar();

        if(currentFieldEnclosure.isEmpty() && field==delimiter){
            return "";
        }

        if(isEOF()){
            return field;
        }

        if(field.size()>32767){ //do not read too much data if user accidentally selected large binary file
            return field;
        }

        //append newline character only if met between enclosure characters
        if(c!='\n' || (!currentFieldEnclosure.isEmpty() && !stopOnDelimiter)){
            field.append(c);
        }else{
            *readEndOfLine = true;
        }

        if(!currentFieldEnclosure.isEmpty() &&
                (field.endsWith(currentFieldEnclosure) || field.endsWith('\\'))){

            bool isBackslashEscape = field.endsWith('\\');

            int escapeStartPos = isBackslashEscape ? field.length() - 1 : field.length() - currentFieldEnclosure.size();
            int escapeLength = isBackslashEscape ? 1 : currentFieldEnclosure.size();

            buffer.append( readString(currentFieldEnclosure.size()) );
            field.append(buffer);

            int readFrom = qMax(field.length()-currentFieldEnclosure.length(),0);
            int readCount = currentFieldEnclosure.length();
            QString afterEscape = field.mid(readFrom, readCount);
            //qDebug() << "before enclosure =" << beforeEnclosure;
            if(afterEscape!=currentFieldEnclosure){ //it's not an escape character (is a real enclosure), need to stop on next delimiter
                stopOnDelimiter = true;
                field.chop(2); //enclosure + afterEscape

                if(field.isEmpty()){
                    field.append(readString(delimiter.size()));
                }
            }else{ //remove escape character
                field.remove(escapeStartPos, escapeLength);
                buffer.chop(currentFieldEnclosure.size());
            }
        }

        stop = isEOF() || ((field.endsWith(delimiter) || c=='\n') && stopOnDelimiter);

    }while(!stop);

    if(field.endsWith(delimiter)){
        field.chop(delimiter.size());
    }

    return field;
}

void CsvImporter::setEncoding(const QString &encoding)
{
    this->encoding = encoding;
}

void CsvImporter::setDelimiter(const QString &delimiter)
{
    this->delimiter = delimiter;
}

void CsvImporter::setSkipRows(int skipRows)
{
    this->skipRows = qMax(skipRows, 0);
}

void CsvImporter::setHeaderOption(CsvImporter::HeaderOption headerOption)
{
    this->headerOption = headerOption;
}

void CsvImporter::setEnclosures(const QStringList &enclosures)
{
    this->enclosures = enclosures;
}

void CsvImporter::resetPosition()
{
    Q_ASSERT(textStream);

    textStream->resetStatus();
    textStream->seek(0);
    buffer.clear();
}

bool CsvImporter::isEOF() const
{
    return textStream->status() == QTextStream::ReadPastEnd;
}
