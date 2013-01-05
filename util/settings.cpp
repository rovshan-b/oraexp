#include "settings.h"

extern QSettings Settings::settings;

void Settings::setValue(const QString &key, const QVariant &value)
{
    settings.setValue(key, value);
}

QVariant Settings::value(const QString &key, const QVariant &defaultValue)
{
    return settings.value(key, defaultValue);
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
