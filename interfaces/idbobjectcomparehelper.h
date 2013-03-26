#ifndef IDBOBJECTCOMPAREHELPER_H
#define IDBOBJECTCOMPAREHELPER_H

class IDbObjectCompareHelper
{
public:
    IDbObjectCompareHelper() {stopped=false;}
    virtual ~IDbObjectCompareHelper(){}

    virtual void compare()=0;
    virtual void stop() {stopped=true;}

protected:
    bool stopped;
};

#endif // IDBOBJECTCOMPAREHELPER_H
