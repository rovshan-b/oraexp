#ifndef OCIEXCEPTION_H
#define OCIEXCEPTION_H

#include <stdexcept>
#include <QString>

class OciException : public std::exception
{
public:
    OciException();
    explicit OciException(const QString &message, int errorCode=0, unsigned int errorRow=-1, unsigned int errorPos=-1);
    virtual ~OciException() throw() {}
    QString getErrorMessage() const;
    int getErrorCode() const;
    unsigned int getErrorRow() const;
    unsigned int getErrorPos() const;

    virtual const char* what() throw();

    void addToErrorMessage(const QString &message, bool prepend=true);

    bool isEmpty(){return this->empty;}

private:
    QString errorMessage;
    int errorCode;
    unsigned int errorRow;
    unsigned int errorPos;

    bool empty;
};

#endif // OCIEXCEPTION_H
