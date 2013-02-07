#include "ociexception.h"

OciException::OciException() : empty(true)
{

}

OciException::OciException(const QString &message, int errorCode, unsigned int errorRow, unsigned int errorPos) :
    errorMessage(message),
    errorCode(errorCode),
    errorRow(errorRow),
    errorPos(errorPos),
    empty(false)
{
}

QString OciException::getErrorMessage() const
{
    return errorMessage;
}

int OciException::getErrorCode() const
{
    return errorCode;
}

unsigned int OciException::getErrorRow() const
{
    return errorRow;
}

unsigned int OciException::getErrorPos() const
{
    return errorPos;
}

void OciException::setErrorPos(unsigned int pos)
{
    this->errorPos=pos;
}

const char* OciException::what() throw()
{
    return this->errorMessage.toStdString().c_str();
}

void OciException::addToErrorMessage(const QString &message, bool prepend)
{
    if(prepend){
        errorMessage.prepend(message);
    }else{
        errorMessage.append(message);
    }
}
