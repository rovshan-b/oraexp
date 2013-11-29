#include "modelutil.h"
#include <QAbstractItemModel>

ModelUtil::ModelUtil()
{
}

QList<QStringList> ModelUtil::getModelData(QAbstractItemModel *model)
{
    QList<QStringList> result;

    for(int i=0; i<model->rowCount(); ++i){

        QStringList colData;
        for(int k=0; k<model->columnCount(); ++k){
            colData.append(model->index(i, k).data().toString());
        }

        result.append(colData);
    }

    return result;
}
