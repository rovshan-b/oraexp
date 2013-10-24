#include "dbconnectioninfo.h"
#include "util/settings.h"
#include "util/strutil.h"

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
    Settings::setValue("savePassword", savePassword);
    Settings::setValue("password", savePassword ? encryptString(password) : QString());
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
    savePassword = Settings::value("savePassword").toBool();
    password = savePassword ? decryptString(Settings::value("password").toString()) : QString();
    type = Settings::value("type").toInt();
    connectionString = Settings::value("connectionString").toString();
    connectAs = (OraExp::ConnectAs) Settings::value("connectAs").toInt();
}
