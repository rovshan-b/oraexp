#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

class Settings
{
public:
    static void setValue ( const QString & key, const QVariant & value );
    static QVariant value ( const QString & key, const QVariant & defaultValue = QVariant() );
    static bool contains (const QString & key);

    static void beginGroup ( const QString & prefix );
    static void endGroup ();

    static int beginReadArray ( const QString & prefix );
    static void beginWriteArray ( const QString & prefix, int size = -1 );
    static void setArrayIndex ( int i );
    static void endArray ();
private:
    Settings();

    static QSettings settings;
};

#endif // SETTINGS_H
