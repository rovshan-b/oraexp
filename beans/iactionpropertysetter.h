#ifndef IACTIONPROPERTYSETTER_H
#define IACTIONPROPERTYSETTER_H

class DynamicAction;

class IActionPropertySetter
{
public:
    virtual void setActionProperties(DynamicAction *action) = 0;
};

#endif // IACTIONPROPERTYSETTER_H
