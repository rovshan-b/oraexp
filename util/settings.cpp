#include "settings.h"

QSettings Settings::settings("OraExp", "Oracle Explorer");

void Settings::setValue(const QString &key, const QVariant &value)
{
    settings.setValue(key, value);
}

QVariant Settings::value(const QString &key, const QVariant &defaultValue)
{
    return settings.value(key, defaultValue);
}

bool Settings::contains(const QString &key)
{
    return settings.contains(key);
}

void Settings::beginGroup(const QString &prefix)
{
    settings.beginGroup(prefix);
}

void Settings::endGroup()
{
    settings.endGroup();
}

Settings::Settings()
{
}
