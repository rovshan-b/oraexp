#ifndef ENVIRONMENTINFO_H
#define ENVIRONMENTINFO_H

#include <QString>
#include <QIcon>
#include "enums.h"

class EnvironmentInfo
{
public:
    EnvironmentInfo();

    QString title;
    QIcon icon;
    OraExp::ConnectionEnvironment environment;

};

#endif // ENVIRONMENTINFO_H
