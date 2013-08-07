#ifndef IREDUCELISTENER_H
#define IREDUCELISTENER_H

class ParsingTable;

class IReduceListener
{
public:
    virtual void reduced(int ruleId, int symbolCount, ParsingTable *parsingTable)=0;
};

#endif // IREDUCELISTENER_H
