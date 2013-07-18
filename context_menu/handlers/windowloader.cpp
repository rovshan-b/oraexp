#include "windowloader.h"
#include "connection_page/dynamic_window/dynamicconnectionpagewindow.h"
#include "util/filesystemutil.h"
#include "util/iconutil.h"
#include "util/dbutil.h"
#include "beans/dynamicwidgetinfo.h"
#include <QDomDocument>

WindowLoader::WindowLoader() :
    ContextMenuHandler()
{
}

void WindowLoader::handle(const QHash<QString, QString> &properties)
{
    DynamicConnectionPageWindow *window = createDynamicWindow(properties);
    QString itemType = properties.value("objectType");

    QPixmap windowIcon;

    DynamicWindowInfo *windowInfo = window->getWindowInfo();
    if(!windowInfo->icon.isEmpty()){
        windowIcon = IconUtil::getIcon(windowInfo->icon);
    }else if(!itemType.isEmpty()){
        windowIcon = IconUtil::getIcon(DbUtil::getDbObjectIconNameByParentNodeType((DbTreeModel::DbTreeNodeType)itemType.toInt()));
    }

    uiManager->addWindow(window, windowIcon, window->getWindowInfo()->caption);
}

DynamicConnectionPageWindow *WindowLoader::createDynamicWindow(const QHash<QString, QString> &properties)
{
    QString windowName = properties.value("attribute.windowName");
    Q_ASSERT(!windowName.isEmpty());

    DynamicWindowInfo *windowInfo = readWindowInfo(windowName);

    DynamicConnectionPageWindow *window = new DynamicConnectionPageWindow(this->uiManager);
    window->setWindowInfo(windowInfo);
    window->setActionProperties(properties);

    return window;
}

DynamicWindowInfo *WindowLoader::readWindowInfo(const QString windowName)
{
    DynamicWindowInfo *info = new DynamicWindowInfo();

    QString fileName = QString(":/dynamic_windows/%1.xml").arg(windowName);

    QString errorMessage;
    QString fileContents = FileSystemUtil::readAsString(fileName, &errorMessage);

    Q_ASSERT(!fileContents.isEmpty() && errorMessage.isEmpty());

    QDomDocument doc;
    doc.setContent(fileContents, &errorMessage);

    Q_ASSERT(errorMessage.isEmpty());

    QDomElement docElem = doc.documentElement();

    info->caption = docElem.attribute("caption");
    info->icon = docElem.attribute("icon");
    info->type = docElem.attribute("type");
    info->scriptFileName = docElem.attribute("scriptFileName");

    QDomNode n = docElem.firstChild();
    while(!n.isNull()) {
        QDomElement e = n.toElement();
        if(!e.isNull()) {
            if(e.tagName()=="widgets"){
                QDomNodeList widgetNodes = e.childNodes();
                readWidgetList(info, widgetNodes);
            }
        }
        n = n.nextSibling();
    }

    return info;
}

void WindowLoader::readWidgetList(DynamicWindowInfo *windowInfo, const QDomNodeList &widgetNodes)
{
    for(int i=0; i<widgetNodes.size(); ++i){
        QDomNode n = widgetNodes.at(i);
        QDomElement e = n.toElement();
        Q_ASSERT(!e.isNull() && e.tagName() == "widget");

        QDomNamedNodeMap attributes = e.attributes();
        QHash<QString,QString> widgetAttributes;
        for(int i=0; i<attributes.size(); ++i){
            QDomAttr attribute = attributes.item(i).toAttr();
            widgetAttributes[attribute.name()] = attribute.value();
        }

        DynamicWidgetInfo *widgetInfo = new DynamicWidgetInfo();
        widgetInfo->attributes = widgetAttributes;
        widgetInfo->childNodes = e.childNodes();

        windowInfo->widgetInfos.append(widgetInfo);
    }
}
