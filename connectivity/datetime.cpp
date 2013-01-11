#include "datetime.h"
#include "connection.h"

DateTime::DateTime() : ociDt(0)
{
}

DateTime::DateTime(QString &value) : ociDt(0)
{
    while(value.endsWith(':')){
        value.chop(1);
    }
    value=value.trimmed();
    //value will be in following format yyyy-mm-dd hh24:mi:ss. time part is optional
    if(value.size()<10 || value.size()>19){ //value is in invalid format
        return;
    }
    QString formatStr("yyyy-MM-dd");

    if(value.size()>=13){ //hour is specified
        formatStr.append(" HH");
    }

    if(value.size()>=16){
        formatStr.append(":mm");
    }

    if(value.size()==19){
        formatStr.append(":ss");
    }

    dt=QDateTime::fromString(value, formatStr);
}

DateTime::~DateTime()
{
    if(ociDt){
        OCI_DateFree(ociDt);
    }
}

void DateTime::setConnection(Connection *cn)
{
    if(ociDt){
        return;
    }
    ociDt = OCI_DateCreate(cn->getOciConnection());
}

QString DateTime::toString() const
{
    return dt.toString("yyyy-MM-dd HH:mm:ss");
}

void DateTime::copyToOci()
{
    QDate date=dt.date();
    if(date.isValid()){
        OCI_DateSetDate(ociDt, date.year(), date.month(), date.day());
    }

    QTime time=dt.time();
    if(time.isValid()){
        OCI_DateSetTime(ociDt, time.hour(), time.minute(), time.second());
    }
}

void DateTime::copyFromOci()
{
    if(OCI_DateCheck(ociDt)){ //returns 0 on success
        return;
    }
    int year, month, day, second, hour, minute;
    OCI_DateGetDateTime(ociDt, &year, &month, &day, &hour, &minute, &second);

    QDate date(year, month, day);
    QTime time(hour, minute, second);

    dt.setDate(date);
    dt.setTime(time);
}

bool DateTime::isValid() const
{
    QTime time=dt.time();
    return dt.date().isValid() && (time.isValid() || time.isNull());
}
