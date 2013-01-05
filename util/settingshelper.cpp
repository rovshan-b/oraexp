#include "settingshelper.h"
#include "util/settings.h"
#include <QtGui>

SettingsHelper::SettingsHelper()
{
}

void SettingsHelper::saveWindowPosition(QWidget *window, const QString &title)
{
    Settings::beginGroup(title);
    if(window->windowState()==Qt::WindowMaximized){
        Settings::setValue("maximized", true);
    }else{
        Settings::setValue("maximized", false);
        Settings::setValue("size", window->size());
        Settings::setValue("pos", window->pos());
    }
    Settings::endGroup();
}

void SettingsHelper::restoreWindowPosition(QWidget *window, const QString &title)
{
    Settings::beginGroup(title);
    bool maximized=Settings::value("maximized", true).toBool();
    window->resize(Settings::value("size", QSize(1024, 700)).toSize());
    window->move(Settings::value("pos", QPoint(0, 0)).toPoint());
    if(maximized){
        window->setWindowState(Qt::WindowMaximized);
    }
    Settings::endGroup();
}

void SettingsHelper::saveMainWindowState(QMainWindow *window, const QString &title)
{
    Settings::beginGroup(title);
    Settings::setValue("windowState", window->saveState());
    Settings::endGroup();
}

void SettingsHelper::restoreMainWindowState(QMainWindow *window, const QString &title)
{
    Settings::beginGroup(title);
    window->restoreState(Settings::value("windowState").toByteArray());
    Settings::endGroup();
}
