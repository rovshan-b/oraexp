#ifndef IREDUCELISTENER_H
#define IREDUCELISTENER_H

class IReduceListener
{
public:
    virtual void reduced(int ruleId, int symbolCount)=0;
};

#endif // IREDUCELISTENER_H
