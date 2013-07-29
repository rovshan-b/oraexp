#ifndef SETTINGSHELPER_H
#define SETTINGSHELPER_H

#include <QString>

class QWidget;
class QMainWindow;
class QMenu;
class DbConnectionInfo;

class SettingsHelper
{
public:
    static void saveWindowPosition(QWidget *window, const QString &title);
    static void restoreWindowPosition(QWidget *window, const QString &title);

    //static void saveMainWindowState(QMainWindow *window, const QString &title);
    //static void restoreMainWindowState(QMainWindow *window, const QString &title);

    static void saveStaticApplicationSettings();
    static void loadStaticApplicationSettings();


    static QList<DbConnectionInfo*> loadConnectionList();
    static void saveConnectionList(QList<DbConnectionInfo *> connectionList);
private:
    SettingsHelper();
};

#endif // SETTINGSHELPER_H
