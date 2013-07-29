#ifndef DBCONNECTIONINFO_H
#define DBCONNECTIONINFO_H

#include <QString>
#include "enums.h"

class DbConnectionInfo
{
public:
    DbConnectionInfo();

    QString uuid;
    QString title;
    OraExp::ConnectionEnvironment environment;
    QString username;
    QString password;
    bool savePassword;
    int type;
    QString connectionString;
    OraExp::ConnectAs connectAs;

    void saveToSettings();
    void readFromSettings();
};

#endif // DBCONNECTIONINFO_H
