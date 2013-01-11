#ifndef DATETIME_H
#define DATETIME_H

#include <QDateTime>
#include "ocilib.h"

class Connection;

class DateTime
{
public:
    DateTime();
    DateTime(QString &value);

    ~DateTime();

    void setConnection(Connection *cn);
    OCI_Date *ociDate() const {return this->ociDt;}

    QString toString() const;

    void copyToOci();
    void copyFromOci();

    bool isValid() const;

private:
    QDateTime dt;
    OCI_Date *ociDt;
};

#endif // DATETIME_H
