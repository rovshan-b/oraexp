#ifndef INSERTEXPORTER_H
#define INSERTEXPORTER_H

#include "dataexporterbase.h"

class InsertExporter : public DataExporterBase
{
public:
    InsertExporter();

    QString schemaName;
    QString tableName;

    virtual void startDocument(QTextStream &out);
    virtual void exportRow(const QStringList &oneRow, int rowIx, QTextStream &out);

protected:
    virtual void prepareField(QString &fieldValue, int fieldIx);

private:
    QString insertStatement;
};

#endif // INSERTEXPORTER_H
