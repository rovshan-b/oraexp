#include "dbconnectioninfo.h"
#include "util/settings.h"

DbConnectionInfo::DbConnectionInfo()
{
}

void DbConnectionInfo::saveToSettings()
{
    Settings::setValue("version", 1);
    Settings::setValue("uuid", uuid);
    Settings::setValue("title", title);
    Settings::setValue("environment", (int)environment);
    Settings::setValue("username", username);
    Settings::setValue("password", savePassword ? password : QString());
    Settings::setValue("savePassword", savePassword);
    Settings::setValue("type", type);
    Settings::setValue("connectionString", connectionString);
    Settings::setValue("connectAs", (int)connectAs);
}

void DbConnectionInfo::readFromSettings()
{
    uuid = Settings::value("uuid").toString();
    title = Settings::value("title").toString();
    environment = (OraExp::ConnectionEnvironment) Settings::value("environment").toInt();
    username = Settings::value("username").toString().toUpper();
    password = Settings::value("password").toString();
    savePassword = Settings::value("savePassword").toBool();
    type = Settings::value("type").toInt();
    connectionString = Settings::value("connectionString").toString();
    connectAs = (OraExp::ConnectAs) Settings::value("connectAs").toInt();
}
