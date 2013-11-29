#ifndef MODELUTIL_H
#define MODELUTIL_H

#include <QStringList>

class QAbstractItemModel;

class ModelUtil
{
public:
    static QList< QStringList > getModelData(QAbstractItemModel *model);

private:
    ModelUtil();
};

#endif // MODELUTIL_H
