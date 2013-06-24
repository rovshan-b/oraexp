#include "savepreviewwidget.h"
#include "widgets/subtabwidget.h"
#include "widgets/codeeditorandsearchpanewidget.h"
#include "util/iconutil.h"
#include "util/widgethelper.h"
#include <QtGui>

SavePreviewWidget::SavePreviewWidget(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout();

    tab = new SubTabWidget();
    tab->setDocumentMode(false);
    mainLayout->addWidget(tab);

    noPreviewWidget = WidgetHelper::createMessageWidget(tr("No preview available"));
    noPreviewWidget->setVisible(false);
    mainLayout->addWidget(noPreviewWidget);

    mainLayout->setContentsMargins(0,0,0,0);
    setLayout(mainLayout);
}

void SavePreviewWidget::setPreviewDocuments(QList<QTextDocument *> documents)
{
    int count = documents.size();
    ensureTabCount(count);

    if(count == 0){
        showNoPreviewMessage(true);
    }else{
        for(int i=0; i<count; ++i){
            CodeEditorAndSearchPaneWidget *editor = static_cast<CodeEditorAndSearchPaneWidget*>(tab->widget(i));
            editor->setTextDocument(documents.at(i));
        }
        showNoPreviewMessage(false);
    }
}

void SavePreviewWidget::clearPreviewDocuments()
{
    showNoPreviewMessage(true);
}

void SavePreviewWidget::ensureTabCount(int count)
{
    setUpdatesEnabled(false);

    int currentCount = tab->count();

    if(count>currentCount){
        for(int i=currentCount; i<count; ++i){
            CodeEditorAndSearchPaneWidget *editor = new CodeEditorAndSearchPaneWidget();
            editor->setReadOnly(true);
            tab->addTab(editor, IconUtil::getIcon("query"), tr("Preview"));
        }
    }else if(count<currentCount){
        for(int i=currentCount-1; i>=count; --i){
            QWidget *widget = tab->widget(i);
            tab->removeTab(i);
            widget->deleteLater();
        }
    }

    //tab->setTabBarVisible(count > 1);

    setUpdatesEnabled(true);
}

void SavePreviewWidget::showNoPreviewMessage(bool show)
{
    noPreviewWidget->setVisible(show);
    tab->setVisible(!show);
}
