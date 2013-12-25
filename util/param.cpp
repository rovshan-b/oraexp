#include "param.h"
#include "strutil.h"
#include "connectivity/statement.h"
#include <QAtomicInt>
#include <QDebug>

#ifdef DEBUG
QAtomicInt Param::objectCount;
#endif

#define MIN_STR_OUT_PARAM_SIZE 500

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
    }else if(type==StringList){
        dtext *charArray=(dtext*)data;
        delete[] charArray;
    }
}

Param *Param::clone()
{
    Param *result;

    switch(type){
    case String:
        result = new Param(paramName, getStrValue(), direction);
        break;
    case Integer:
        result = new Param(paramName, (int)getIntValue(), direction);
        break;
    case Double:
        result = new Param(paramName, (double)getDoubleValue(), direction);
        break;
    case Datetime:
    {
        QString currVal = getDateTimeValue()->toString();
        result = new Param(paramName, DateTime(currVal), direction);
        break;
    }
    case Stmt:
        result = new Param(paramName);
        break;
    case ReturningInto:
        result = new Param(paramName, ReturningInto);
        break;
    default:
        //cloning string lists not supported for now. will implement when required.
        Q_ASSERT(false);
        break;
    }

    return result;
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

Param::Param(const QString &paramName, const QStringList &paramValue, int maxLength, ParamDirection direction)
{
    int calcMaxLength = maxLength;
    if(calcMaxLength==-1){
        for(int i=0; i<paramValue.size(); ++i){
            calcMaxLength = qMax(calcMaxLength, paramValue.at(i).size());
        }
    }
    dtext *charArray = new dtext[paramValue.size()*(calcMaxLength+1)];
    this->arraySize = paramValue.size();
    this->maxStringLength = calcMaxLength;

    for(int i=0; i<paramValue.size(); ++i){
        copyStringToArray(charArray, i, calcMaxLength, paramValue.at(i).toStdWString().c_str());
    }

    initParam(paramName, StringList, direction, charArray);
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
    {
        const QString &strVal = *((QString*)paramValue);
        int strLength = strVal.length();
        if(direction == Param::In || strLength >= MIN_STR_OUT_PARAM_SIZE){
            data=toOciString(strVal);
            maxStringLength = strLength;
        }else if(strLength < MIN_STR_OUT_PARAM_SIZE){ //InOut or Out. Allocate 500 bytes
            data = new dtext[MIN_STR_OUT_PARAM_SIZE + 1];
            strVal.toWCharArray((dtext*)data);
            ((dtext*)data)[strVal.size()]=L'\0';
            maxStringLength = MIN_STR_OUT_PARAM_SIZE;
        }
        break;
    }
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
    case StringList:
        data  = (dtext*)paramValue; //already allocated in calling function
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
    maxStringLength = paramValue.size();
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
    case StringList:
        result = QObject::tr("Array");
        break;
    default:
        Q_ASSERT(false);
        break;
    }

    return result;
}

QList<Param *> Param::cloneParams(const QList<Param *> params)
{
    QList<Param*> result;
    result.reserve(params.size());

    for(int i=0; i<params.size(); ++i){
        result.append(params.at(i)->clone());
    }

    return result;
}

void Param::printObjectCount()
{
#ifdef DEBUG
    qDebug() << "Total number of params:" << (int)Param::objectCount;
#endif
}
