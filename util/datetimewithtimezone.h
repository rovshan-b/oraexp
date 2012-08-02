#ifndef DATETIMEWITHTIMEZONE_H
#define DATETIMEWITHTIMEZONE_H

#include <QDateTime>

class DateTimeWithTimezone : public QDateTime
{
public:
    DateTimeWithTimezone(const QDate & date, const QTime & time,
                         int tzhour, int tzminute);

    QString getTzInfoAsString() const;
    QString toStringWithTzInfo() const;

private:
    int tzhour;
    int tzminute;

};

#endif // DATETIMEWITHTIMEZONE_H
