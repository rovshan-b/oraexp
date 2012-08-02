#include "datetimewithtimezone.h"

DateTimeWithTimezone::DateTimeWithTimezone(const QDate & date, const QTime & time,
                                           int tzhour, int tzminute) :
    QDateTime(date, time)
{
    this->tzhour=tzhour;
    this->tzminute=tzminute;
}

QString DateTimeWithTimezone::getTzInfoAsString() const
{
    QString res=QString("%1%2:%3").arg(tzhour>0 ? "+" : "").
                arg(tzhour, 2, 10, QChar('0')).
                arg(tzminute, 2, 10, QChar('0'));

    return res;
}

QString DateTimeWithTimezone::toStringWithTzInfo() const
{
    return toString("yyyy-MM-dd hh:mm:ss.zzz ").append(getTzInfoAsString());
}
