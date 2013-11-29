#ifndef PARAM_H
#define PARAM_H

#include <QString>
#include <QVariant>
#include "connectivity/datetime.h"

class Statement;

class Param
{
public:
    enum ParamDirection {
        In,
        Out,
        InOut
    };

    enum ParamType {
        String,
        Integer,
        Double,
        Datetime,
        Stmt,
        ReturningInto,
        StringList
    };

    ~Param();
    Param(const QString &paramName, const QString &paramValue, ParamDirection direction=In);
    Param(const QString &paramName, int paramValue, ParamDirection direction=In);
    Param(const QString &paramName, double paramValue, ParamDirection direction=In);
    Param(const QString &paramName, bool paramValue, ParamDirection direction=In);
    Param(const QString &paramName, const DateTime &paramValue, ParamDirection direction=In);
    Param(const QString &paramName);
    Param(const QString &paramName, ParamType paramType, void *paramValue = 0, ParamDirection direction=In);

    Param(const QString &paramName, const QStringList &paramValue, int maxLength = -1, ParamDirection direction=In);

    Param *clone();

    QString getParamName() const;

    QString getStrValue() const;
    void setStrValue(const QString &paramValue);
    int getMaxStringLength() const {return this->maxStringLength;}

    int getIntValue() const;
    void setIntValue(int paramValue);

    int getDoubleValue() const;
    void setDoubleValue(int paramValue);

    DateTime *getDateTimeValue() const;

    Statement *getStmtValue() const;

    ParamDirection getParamDirection() const {return this->direction;}
    ParamType getParamType() const {return this->type;}

    void setNull(){this->isNullParam=true;}
    bool isNull() const {return this->isNullParam;}

    QString toString() const;

    int getArraySize() const {return this->arraySize;}

    void *data;

    static QList<Param*> cloneParams(const QList<Param*> params);

    static void printObjectCount();
private:
    QString paramName;

    ParamDirection direction;
    ParamType type;

    int arraySize;
    int maxStringLength;

    void cleanup();

    bool isNullParam;

    void initParam(const QString &paramName,
                   Param::ParamType paramType,
                   Param::ParamDirection direction,
                   void *paramValue = 0);

#ifdef DEBUG
    static QAtomicInt objectCount;
#endif

    void ref()
    {
        isNullParam=false;
#ifdef DEBUG
        Param::objectCount.ref();
#endif
    }

    void deref()
    {
#ifdef DEBUG
        Param::objectCount.deref();
#endif
    }
};

#endif // PARAM_H
