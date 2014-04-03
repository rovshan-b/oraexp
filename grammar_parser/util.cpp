#include "util.h"

bool caseInsensitiveLessThan(const QString &s1, const QString &s2)
{
    return (QString::compare(s1, s2, Qt::CaseInsensitive) < 0);
}
