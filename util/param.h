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
        Stmt
    };

    ~Param();
    Param(const QString &paramName, const QString &paramValue, ParamDirection direction=InOut);
    Param(const QString &paramName, int paramValue, ParamDirection direction=InOut);
    Param(const QString &paramName, double paramValue, ParamDirection direction=InOut);
    Param(const QString &paramName, bool paramValue, ParamDirection direction=InOut);
    Param(const QString &paramName, const DateTime &paramValue, ParamDirection direction=InOut);
    Param(const QString &paramName);

    QString getParamName() const;

    QString getStrValue() const;
    void setStrValue(const QString &paramValue);

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

    void *data;

    static void printObjectCount();
private:
    QString paramName;

    ParamDirection direction;
    ParamType type;

    void cleanup();

    bool isNullParam;

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
