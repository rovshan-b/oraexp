#include "strutil.h"
#include <QRegExp>
#include <iostream>
#include <QCryptographicHash>
#include <QLocale>
#include "dbutil.h"

using namespace std;

QString toQString(const dtext *ociString){
    return QString::fromWCharArray(ociString);
}

dtext* toOciString(const QString &qString){
    dtext *data = new dtext[qString.size()+1]; //must be freed by caller
    qString.toWCharArray(data);
    data[qString.size()]=L'\0';

    return data;
}

int getEditorIntValue(const QString &value)
{
    return value.isEmpty() ? -1 : value.toInt();
}

QString getEditorStrValue(int value)
{
    return value==-1 ? "" : QString::number(value);
}

QStringList splitTrimmed(const QString &text, bool convertToUpperCase)
{
    QStringList result;
    if(!text.isEmpty()){
        QStringList list=text.split(',');
        for(int i=0; i<list.count(); ++i){
            if(convertToUpperCase){
                result.append(list.at(i).trimmed().toUpper());
            }else{
                result.append(list.at(i).trimmed());
            }
        }
    }
    return result;
}

QString humanize(const QString &input)
{
    if(input.isEmpty()){
        return input;
    }

    QString result = input.toLower();

    result[0]=result.at(0).toUpper();
    int underscoreIx=result.indexOf('_');
    while(underscoreIx!=-1){
        if(result.length()>underscoreIx/*+1*/){
            result[underscoreIx]=' ';
            //result[underscoreIx+1]=result.at(underscoreIx+1).toUpper();
        }

        underscoreIx=result.indexOf('_', underscoreIx+1);
    }

    return result;
}

QString unhumanize(const QString &input)
{
    QString result = input;
    return result.toUpper().replace(' ', '_');
}

QStringList humanizeList(const QStringList &input)
{
    QStringList result;

    foreach(const QString &str, input){
        result.append(humanize(str));
    }

    return result;
}

QStringList unhumanizeList(const QStringList &input)
{
    QStringList result;

    foreach(const QString &str, input){
        result.append(unhumanize(str));
    }

    return result;
}

QString humanizeSize(int size)
{
    QString result;
    if(size>=(1024*1024*1024)){ //gb
        result = QString::number((double)size/1024/1024/1024, 'f', 2).append(QObject::tr(" G"));
    }else if(size>=(1024*1024)){ //mb
        result = QString::number((double)size/1024/1024, 'f', 2).append(QObject::tr(" M"));
    }else if(size>=(1024)){ //kb
        result = QString::number((double)size/1024, 'f', 2).append(QObject::tr(" K"));
    }else{
        result = QString::number(size).append(QObject::tr(" B"));
    }

    return result;
}

QString optionText(int option, const QString &option1,
                               const QString &option2)
{
    if(option==1){
        return option1;
    }else if(option==2){
        return option2;
    }else{
        return QString();
    }
}

QString optionText(int option, const QString &option1,
                               const QString &option2,
                               const QString &option3)
{
    if(option==1){
        return option1;
    }else if(option==2){
        return option2;
    }else if(option==3){
        return option3;
    }else{
        return QString();
    }
}

int numberAfterLastUnderscore(const QString &s)
{
    bool ok;
    int result=s.mid(s.lastIndexOf('_')+1).toInt(&ok);
    Q_ASSERT(ok);

    return result;
}

bool existsInCollection(const QString &commaSeparatedString, const QStringList &collection)
{
    QStringList splittedList=commaSeparatedString.split(',');
    foreach(const QString &splittedItem, splittedList){
        if(collection.contains(splittedItem)){
            return true;
        }
    }

    return false;
}

bool allExistInCollection(const QString &commaSeparatedString, const QStringList &collection)
{
    QStringList splittedList=commaSeparatedString.split(',');
    foreach(const QString &splittedItem, splittedList){
        if(!collection.contains(splittedItem)){
            return false;
        }
    }

    return true;
}

bool hasMatch(const QString &str, const QStringList &collection)
{
    foreach(const QString &item, collection){
        QRegExp r(item, Qt::CaseSensitive, QRegExp::RegExp2);
        if(r.exactMatch(str)){
            return true;
        }
    }

    return false;
}

void addEOL(QString &str, const QString &terminator)
{
    if(!str.isEmpty()){
        if(!terminator.isEmpty()){
            str.append(terminator);
        }
        str.append("\n");
    }
}

QString formatSecs(int secs)
{
    int mins = secs / 60;
    int hours = mins / 3600;
    int remainingSecs = secs % 60;

    return QString("%1:%2:%3")
    .arg(hours, 2, 10, QLatin1Char('0'))
    .arg(mins, 2, 10, QLatin1Char('0'))
    .arg(remainingSecs, 2, 10, QLatin1Char('0'));
}

QString formatMsecs(int msecs, bool includeMsecs)
{
    int secs = msecs / 1000;
    int mins = (secs / 60) % 60;
    int hours = (secs / 3600);
    secs = secs % 60;

    if(includeMsecs){
        return QString("%1:%2:%3.%4")
        .arg(hours, 2, 10, QLatin1Char('0'))
        .arg(mins, 2, 10, QLatin1Char('0'))
        .arg(secs, 2, 10, QLatin1Char('0'))
        .arg(msecs%1000, 3, 10, QLatin1Char('0'));
    }else{
        return QString("%1:%2:%3")
        .arg(hours, 2, 10, QLatin1Char('0'))
        .arg(mins, 2, 10, QLatin1Char('0'))
        .arg(secs, 2, 10, QLatin1Char('0'));
    }
}

QString joinEnclosed(const QStringList &list, const QString &join, const QString &enclose)
{
    QString result;

    for(int i=0; i<list.size(); ++i){
        if(i>0){
            result.append(join);
        }
        if(!(result.startsWith(enclose) && result.endsWith(enclose))){
            result.append(enclose).append(list.at(i)).append(enclose);
        }else{
            result.append(list.at(i));
        }
    }

    return result;
}

QString enclose(const QString &str, const QString &enclose)
{
    if(!(str.startsWith(enclose) && str.endsWith(enclose))){
        return QString("%1%2%1").arg(enclose, str);
    }else{
        return str;
    }
}

QString ensureStartsWith(QString str, const QString &startWith)
{
    QRegExp regexp(QString("^\b%1\b").arg(startWith), Qt::CaseInsensitive, QRegExp::RegExp2);
    int alreadyStartsWith=regexp.indexIn(str.replace("\n"," "));
    if(alreadyStartsWith>-1){
        return str;
    }else{
        return QString(" %1 %2").arg(startWith, str);
    }
}

void print(const char *msg)
{
    cout << msg << endl;
}

bool caseInsensitiveLessThan(const QString &s1, const QString &s2)
{
    return (QString::compare(s1, s2, Qt::CaseInsensitive) < 0);
}

bool stringLengthDescLessThan(const QString &s1, const QString &s2)
{
    return s1.length() > s2.length();
}

void setDbLinkName(QString &str, const QString &dbLinkName)
{
    if(!dbLinkName.isEmpty()){
        str.replace("{db_link}", QString("@\"%1\"").arg(dbLinkName));
    }else{
        str.remove("{db_link}");
    }
}

QString md5(const QString &str)
{
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(str.toUtf8());
    return hash.result().toHex();
}

QString addNumericSuffix(const QString &str, const QStringList &excludeList, int startSuffix)
{
    QString result;

    int suffix=startSuffix;

    do{
        QString check=QString("%1_%2").arg(str).arg(++suffix);
        if(!excludeList.contains(check)){
            result = check;
            break;
        }
    }while(true);

    return result;
}

void replaceGuiEscapeCharactersWithReal(QString &str)
{
    str.replace("\\n", "\n").replace("\\\n", "\\n").
        replace("\\r", "\r").replace("\\\r", "\\r").
        replace("\\t", "\t").replace("\\\t", "\\t");
}

QString xmlEncode(const QString& xml)
{
    QString temp;
    temp.reserve(xml.size());

    for (int index = 0; index < xml.size(); index++)
    {
        QChar character(xml.at(index));

        switch (character.unicode())
        {
        case '&':
            temp.append("&amp;");
            break;
        case '\'':
            temp.append("&apos;");
            break;
        case '"':
            temp.append("&quot;");
            break;
        case '<':
            temp.append("&lt;");
            break;
        case '>':
            temp.append("&gt;");
            break;
        default:
            temp.append(character);
            break;
        }
    }

    return temp;
}

QString removeEnclosure(const QString &str, const QChar &enclosure)
{
    QString s = str;

    if(s.length()>2 && s.startsWith(enclosure) && s.endsWith(enclosure)){
        s.remove(0, 1).chop(1);
    }

    return s;
}

void copyStringToArray(dtext *array, int arrayPos, int arrayMaxStrLength, const dtext *str)
{
    dtext *currPosInBuffer = &array[arrayPos*(arrayMaxStrLength+1)];
    dtscpy(currPosInBuffer, str);
}

QString toValidFilename(const QString &str)
{
    QString result(str);
    for(int i=0; i<result.size(); ++i){
        QChar c = result.at(i);
        if(!c.isDigit() && !c.isLetter() && c!='_' && c!='-'){
            result[i]='_';
        }
    }

    return result;
}

#ifndef CODE_PARSER_BUILD

QString detectDateFormat(const QStringList &dates)
{
    Q_ASSERT(!dates.isEmpty());

    QList<QString> datePatterns;
    QList<QString> dateFormats;

    QList<QString> timePatterns;
    QList<QString> timeFormats;

    DbUtil::getDateFormats(&dateFormats, &datePatterns);
    DbUtil::getTimeFormats(&timeFormats, &timePatterns);

    QString format;
    QString date = dates.at(0);
    for(int i=0; i<datePatterns.size(); ++i){

        //first try to match only date part
        QString pattern = datePatterns.at(i);
        QRegExp regexp(pattern, Qt::CaseInsensitive, QRegExp::RegExp2);

        if(regexp.exactMatch(date)){
            format = dateFormatForLocale(dateFormats.at(i), dates);
            format.replace("-", regexp.capturedTexts().at(1));
            break;
        }

        for(int k=0; k<timePatterns.size(); ++k){
            //now do a full match
            pattern = QString("%1 %2").arg(datePatterns.at(i), timePatterns.at(k));
            regexp.setPattern(pattern);

            if(regexp.exactMatch(date)){
                format = QString("%1 %2").arg(dateFormatForLocale(dateFormats.at(i), dates), timeFormats.at(k));
                format.replace("-", regexp.capturedTexts().at(1));
                break;
            }
        }
    }

    return format;
}

QString dateFormatForLocale(const QString &dateFormat, const QStringList &dates)
{
    if(!dateFormat.startsWith("DD-MM")){
        return dateFormat;
    }

    QRegExp regexp("^(\\d{2})(/|-|.)(\\d{2})", Qt::CaseSensitive, QRegExp::RegExp2);
    for(int i=0; i<dates.size(); ++i){
        QString date = dates.at(i);
        regexp.indexIn(date);
        if(regexp.capturedTexts().size()!=4){
            continue;
        }
        int firstNumber = regexp.capturedTexts().at(1).toInt();
        int secondNumber = regexp.capturedTexts().at(3).toInt();

        if(secondNumber>12){ //check if this is US format
            return QString(dateFormat).replace("DD-MM", "MM-DD");
        }else if(firstNumber>12){ //is already in correct format
            return dateFormat;
        }
    }

    //if neither first nor second part has digits higher than 12 then we return locale specific format

    QLocale::Country country = QLocale::system().country();

    if(country == QLocale::UnitedStates || country == QLocale::UnitedStatesMinorOutlyingIslands){
        return QString(dateFormat).replace("DD-MM", "MM-DD");
    }

    return dateFormat;
}

QString formatBindVarName(const QString &varName, int position, OraExp::BindVarStyle bindStyle)
{
    QString result;

    switch(bindStyle){
    case OraExp::BindVarStyleNamed:
        result = QString(":%1").arg(varName);
        break;
    case OraExp::BindVarStylePositioned:
        result = QString(":%1").arg(position);
        break;
    case OraExp::BindVarStyleQuestionMark:
        result = "?";
        break;
    }

    return result;
}

#endif
