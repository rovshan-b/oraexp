#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

class Settings
{
public:
    static void setValue ( const QString & key, const QVariant & value );
    static QVariant value ( const QString & key, const QVariant & defaultValue = QVariant() );

    static void beginGroup ( const QString & prefix );
    static void endGroup ();
private:
    Settings();

    static QSettings settings;
};

#endif // SETTINGS_H
