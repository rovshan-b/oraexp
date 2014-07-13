#ifndef BULKOPERATIONHELPER_H
#define BULKOPERATIONHELPER_H

#include <QStringList>

class Statement;
class Resultset;

class BulkOperationHelper
{
public:
    BulkOperationHelper();
    ~BulkOperationHelper();

    void setBulkSize(int size);
    void setArrayData(Statement *targetStmt, Resultset *sourceRs, int column, int pos);
    void setArrayData(Statement *targetStmt, const QString &value, int column, int pos);
    void nullifyStringAtPos(int bufferIx, int dataIx);
    void copyStringAtPos(int bufferIx, int dataIx, const wchar_t *str);
    void nullifyArrayData(Statement *targetStmt, int offset);
    void bindArrays(Statement *stmt, const QString &bindVarPrefix, const QList<int> &bindVarSuffixes=QList<int>());
    void createBufferForDataType(Statement *stmt, const QString &dataType, int length);

    //void printArrayData(Statement *stmt);

    QString dataTypeAt(int ix);
    int lengthAt(int ix);

    void setDmlMode();
    void cleanup();
private:

    int bulkSize;

    QList<void*> buffers;
    QStringList dataTypes;
    QList<int> lenghts;

    bool dmlMode;

};

#endif // BULKOPERATIONHELPER_H
