#include "settingshelper.h"
#include "util/settings.h"
#include "util/widgethelper.h"
#include "connection_page/connectionpageconnectedwidget.h"
#include "connection_page/worksheet/worksheetwidget.h"
#include "connection_page/code_creator/codecreatorwidget.h"
#include "connection_page/worksheet/bottom_pane_tabs/worksheetexplainplantab.h"
#include "codeeditor/codeeditor.h"
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

/*
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
}*/

void SettingsHelper::saveStaticApplicationSettings()
{
    Settings::setValue("ConnectionPage/windowState", ConnectionPageConnectedWidget::currentState);

    Settings::setValue("WorksheetWidget/splitterSizes", WorksheetWidget::splitterSizes);
    Settings::setValue("WorksheetExplainPlanTab/advancedOptionsVisible", WorksheetExplainPlanTab::advancedOptionsVisible);
    Settings::setValue("WorksheetExplainPlanTab/explainPlanViewIndex", WorksheetExplainPlanTab::explainPlanViewIndex);
    Settings::setValue("WorksheetExplainPlanTab/autotraceViewIndex", WorksheetExplainPlanTab::autotraceViewIndex);

    Settings::setValue("CodeCreatorWidget/bottomSplitterSizes", CodeCreatorWidget::bottomSplitterSizes);
    Settings::setValue("CodeEditor/currentFont", CodeEditor::currentFont.toString());
}

void SettingsHelper::loadStaticApplicationSettings()
{
    ConnectionPageConnectedWidget::currentState = Settings::value("ConnectionPage/windowState").toByteArray();

    WorksheetWidget::splitterSizes = Settings::value("WorksheetWidget/splitterSizes").toByteArray();
    WorksheetExplainPlanTab::advancedOptionsVisible = Settings::value("WorksheetExplainPlanTab/advancedOptionsVisible", false).toBool();
    WorksheetExplainPlanTab::explainPlanViewIndex = Settings::value("WorksheetExplainPlanTab/explainPlanViewIndex", 0).toInt();
    WorksheetExplainPlanTab::autotraceViewIndex = Settings::value("WorksheetExplainPlanTab/autotraceViewIndex", 0).toInt();

    CodeCreatorWidget::bottomSplitterSizes = Settings::value("CodeCreatorWidget/bottomSplitterSizes").toByteArray();
    CodeEditor::currentFont.fromString(Settings::value("CodeEditor/currentFont", WidgetHelper::getMonospaceFont()).toString());
}
