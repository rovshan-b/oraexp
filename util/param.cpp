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
    }else if(type==Stmt){
        Statement *stmt=(Statement*)data;
        delete stmt;
    }
}

Param::Param(const QString &paramName, const QString &paramValue, ParamDirection direction) : paramName(paramName), direction(direction)
{
    type=String;

    data=toOciString(paramValue);

    ref();
}

Param::Param(const QString &paramName, int paramValue, ParamDirection direction) : paramName(paramName), direction(direction)
{
    type=Integer;
    data=new int(paramValue);

    ref();
}

Param::Param(const QString &paramName, double paramValue, Param::ParamDirection direction) : paramName(paramName), direction(direction)
{
    type=Double;
    data=new double(paramValue);

    ref();
}

Param::Param(const QString &paramName, bool paramValue, ParamDirection direction) : paramName(paramName), direction(direction)
{
    type=Integer;
    data=new int(paramValue==0 ? false : true);

    ref();
}

Param::Param(const QString &paramName) : paramName(paramName), direction(InOut)
{
    type=Stmt;
    data=new Statement();

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

Statement *Param::getStmtValue() const
{
    Q_ASSERT(type==Stmt);

    return (Statement*)data;
}

void Param::printObjectCount()
{
#ifdef DEBUG
    qDebug() << "Total number of params:" << (int)Param::objectCount;
#endif
}
