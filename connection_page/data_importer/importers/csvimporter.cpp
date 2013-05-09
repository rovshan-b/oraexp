#include "csvimporter.h"
#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include "interfaces/idataimportlistener.h"

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
            int bufferSize = buffer.size();
            result.append(buffer);
            buffer.clear();
            result.append(textStream->read(count - bufferSize));

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

void CsvImporter::readRows(IDataImportListener *importListener, int maxCount)
{
    int readCount = 0;
    int skippedCount = 0;
    bool readHeader = (headerOption==NoHeader);

    while(!isEOF() && (maxCount == -1 || (readCount++) < maxCount)){
        QStringList values = readValues();

        if(values.isEmpty()){
            continue;
        }

        if(!readHeader && headerOption == BeforeSkip){
            importListener->headerAvailable(values);
            readHeader = true;
            continue;
        }

        if(skippedCount < skipRows){
            ++skippedCount;
            --readCount;
            continue;
        }

        if(!readHeader && headerOption == AfterSkip){
            importListener->headerAvailable(values);
            readHeader = true;
            continue;
        }

        importListener->rowAvailable(values);
    }
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
            field.remove(0, enclosure.size());
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

            QString partToAppend = readString(currentFieldEnclosure.size());
            buffer.append(partToAppend);
            field.append(partToAppend);

            int readFrom = qMax(field.length()-partToAppend.length(),0);
            int readCount = partToAppend.length();
            QString afterEscape = field.mid(readFrom, readCount);
            //qDebug() << "before enclosure =" << beforeEnclosure;
            if(afterEscape!=currentFieldEnclosure){ //it's not an escape character (is a real enclosure), need to stop on next delimiter
                stopOnDelimiter = true;
                field.chop(partToAppend.size()+currentFieldEnclosure.size());

                if(field.isEmpty()){
                    field.append(readString(delimiter.size()));
                }
            }else{ //remove escape character
                field.remove(escapeStartPos, escapeLength);
                buffer.chop(partToAppend.size());
            }
        }

        stop = isEOF() || ((field.endsWith(delimiter) || c=='\n') && stopOnDelimiter);

    }while(!stop);

    if(field.endsWith(delimiter)){
        field.chop(delimiter.size());
    }

    return field;
}

bool CsvImporter::setEncoding(const QString &encoding)
{
    if(encoding == this->encoding){
        return false;
    }

    this->encoding = encoding;
    return true;
}

bool CsvImporter::setDelimiter(const QString &delimiter)
{
    if(delimiter == this->delimiter){
        return false;
    }

    this->delimiter = delimiter;
    return true;
}

bool CsvImporter::setSkipRows(int skipRows)
{
    if(skipRows == this->skipRows){
        return false;
    }

    this->skipRows = qMax(skipRows, 0);
    return true;
}

bool CsvImporter::setHeaderOption(CsvImporter::HeaderOption headerOption)
{
    if(headerOption == this->headerOption){
        return false;
    }

    this->headerOption = headerOption;
    return true;
}

bool CsvImporter::setEnclosures(const QStringList &enclosures)
{
    if(enclosures == this->enclosures){
        return false;
    }

    this->enclosures = enclosures;
    return true;
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
    return textStream->status() == QTextStream::ReadPastEnd && buffer.isEmpty();
}
