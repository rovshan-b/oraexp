#ifndef DATAEXPORTER_H
#define DATAEXPORTER_H

#include "connection_page/data_comparer/datacomparer.h"

class DataExporter : public DataComparer
{
    Q_OBJECT
public:
    explicit DataExporter(DbUiManager *uiManager, QWidget *parent = 0);
    
signals:
    
public slots:
    
};

#endif // DATAEXPORTER_H
