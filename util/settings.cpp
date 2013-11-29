#include "settings.h"

QSettings Settings::settings("OraExp", "OraExp");

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

void Settings::saveStringHash(const QString &key, const QStringHash &hash)
{
    Settings::beginWriteArray(key, hash.size());

    QHashIterator<QString, QString> i(hash);
    int k=0;
    while (i.hasNext()) {
        i.next();

        Settings::setArrayIndex(k++);

        Settings::setValue("key", i.key());
        Settings::setValue("value", i.value());
    }

    Settings::endArray();
}

QStringHash Settings::loadStringHash(const QString &key)
{
    QStringHash result;

    int size = Settings::beginReadArray(key);

    for(int i=0; i<size; ++i){
        Settings::setArrayIndex(i);

        result[Settings::value("key").toString()] = Settings::value("value").toString();
    }

    Settings::endArray();

    return result;
}

Settings::Settings()
{
}
