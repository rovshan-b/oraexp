#ifndef DATETIME_H
#define DATETIME_H

#include <QDateTime>
#include "ocilib.h"

class Connection;

class DateTime
{
public:
    DateTime();
    DateTime(const QString &value);

    ~DateTime();

    void setConnection(Connection *cn);
    OCI_Date *ociDate() const {return this->ociDt;}

    void copyToOci();
    void copyFromOci();

private:
    QDateTime dt;
    OCI_Date *ociDt;
};

#endif // DATETIME_H
