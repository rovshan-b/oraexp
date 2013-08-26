#include "genericobjectcontextmenuutil.h"
#include "util/iconutil.h"
#include "connection_page/dbuimanager.h"
#include "beans/iactionpropertysetter.h"

GenericObjectContextMenuUtil::GenericObjectContextMenuUtil()
{
}

QList<QAction *> GenericObjectContextMenuUtil::getActionsForObject(const QString &schemaName, const QString &objectName,
                                                                   DbUiManager *uiManager, IActionPropertySetter *propertySetter)
{
    if(!propertySetter){
        return QList<QAction*>();
    }

    DbItemAction *viewAction = new DbItemAction(QIcon(), QObject::tr("View"),
                                                schemaName, objectName, DbTreeModel::Unknown,
                                                uiManager, SLOT(createViewer()));
    viewAction->isDefault = true;
    propertySetter->setActionProperties(viewAction);

    QString objectTypeName = viewAction->properties["DB_OBJECT_TYPE"];
    QString iconName = objectTypeName.toLower().replace(' ', '_');
    viewAction->setIcon(IconUtil::getIcon(iconName));

    return QList<QAction*>() << viewAction;
}
