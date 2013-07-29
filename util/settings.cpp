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

int Settings::beginReadArray(const QString &prefix)
{
    return settings.beginReadArray(prefix);
}

void Settings::beginWriteArray(const QString &prefix, int size)
{
    settings.beginWriteArray(prefix, size);
}

void Settings::setArrayIndex(int i)
{
    settings.setArrayIndex(i);
}

void Settings::endArray()
{
    settings.endArray();
}

Settings::Settings()
{
}
