#include "param.h"
#include "strutil.h"
#include "connectivity/statement.h"
#include <QAtomicInt>
#include <QDebug>

#ifdef DEBUG
QAtomicInt Param::objectCount;
#endif

Param::~Param()
{
    cleanup();

    deref();
}

void Param::cleanup()
{
    if(type==String){
        dtext *str=(dtext*)data;
        delete[] str;
    }else if(type==Integer){
        int *i=(int*)data;
        delete i;
    }else if(type==Double){
        double *d=(double*)data;
        delete d;
    }else if(type==Datetime){
        DateTime *dt=(DateTime*)data;
        delete dt;
    }else if(type==Stmt){
        Statement *stmt=(Statement*)data;
        delete stmt;
    }
}


Param::Param(const QString &paramName, const QString &paramValue, ParamDirection direction)
{
    QString copy = paramValue;
    initParam(paramName, String, direction, &copy);
}

Param::Param(const QString &paramName, int paramValue, ParamDirection direction)
{
    initParam(paramName, Integer, direction, &paramValue);
}

Param::Param(const QString &paramName, double paramValue, Param::ParamDirection direction)
{
    initParam(paramName, Double, direction, &paramValue);
}

Param::Param(const QString &paramName, bool paramValue, ParamDirection direction)
{
    int intValue = paramValue ? 1 : 0;
    initParam(paramName, Integer, direction, &intValue);
}

Param::Param(const QString &paramName, const DateTime &paramValue, Param::ParamDirection direction) : paramName(paramName), direction(direction)
{
    DateTime copy = paramValue;
    initParam(paramName, Datetime, direction, &copy);
}

Param::Param(const QString &paramName) : paramName(paramName), direction(Out)
{
    initParam(paramName, Stmt, direction, 0);
}

Param::Param(const QString &paramName, Param::ParamType paramType, void *paramValue, ParamDirection direction)
{
    initParam(paramName, paramType, direction, paramValue);
}

void Param::initParam(const QString &paramName,
                      Param::ParamType paramType,
                      Param::ParamDirection direction,
                      void *paramValue)
{
    this->paramName=paramName;
    this->type=paramType;
    this->direction=direction;

    switch(paramType){
    case String:
        data=toOciString(*((QString*)paramValue));
        break;
    case Integer:
        data=new int(*((int*)paramValue));
        break;
    case Double:
        data=new double(*((double*)paramValue));
        break;
    case Datetime:
        data=new DateTime(*((DateTime*)paramValue));
        break;
    case Stmt:
        data=new Statement();
        break;
    case ReturningInto:
        data=0;
        break;
    default:
        data = 0;
        Q_ASSERT(false);
        break;
    }

    ref();
}

QString Param::getParamName() const
{
    return paramName;
}

QString Param::getStrValue() const
{
    Q_ASSERT(type==String);

    return toQString((dtext*)data);
}

void Param::setStrValue(const QString &paramValue)
{
    Q_ASSERT(type==String);

    cleanup();
    data=toOciString(paramValue);
}

int Param::getIntValue() const
{
    Q_ASSERT(type==Integer);

    return *((int*)data);
}

void Param::setIntValue(int paramValue)
{
    Q_ASSERT(type==Integer);

    *((int*)data)=paramValue;
}

int Param::getDoubleValue() const
{
    Q_ASSERT(type==Double);

    return *((double*)data);
}

void Param::setDoubleValue(int paramValue)
{
    Q_ASSERT(type==Double);

    *((double*)data)=paramValue;
}

DateTime *Param::getDateTimeValue() const
{
    Q_ASSERT(type==Datetime);

    return (DateTime*)data;
}

Statement *Param::getStmtValue() const
{
    Q_ASSERT(type==Stmt);

    return (Statement*)data;
}

QString Param::toString() const
{
    QString result;

    switch(type){
    case String:
        result = getStrValue();
        break;
    case Integer:
        result = QString::number(getIntValue());
        break;
    case Double:
        result = QString::number(getDoubleValue());
        break;
    case Datetime:
        result = getDateTimeValue()->toString();
        break;
    case Stmt:
        result = QObject::tr("Cursor");
        break;
    case ReturningInto:
        result = QObject::tr("Returning into");
        break;
    default:
        Q_ASSERT(false);
        break;
    }

    return result;
}

void Param::printObjectCount()
{
#ifdef DEBUG
    qDebug() << "Total number of params:" << (int)Param::objectCount;
#endif
}
