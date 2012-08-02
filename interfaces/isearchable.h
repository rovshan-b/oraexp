#ifndef ISEARCHABLE_H
#define ISEARCHABLE_H

class ISearchable
{
public:
    virtual ~ISearchable(){}

    virtual void showSearchWidget()=0;
    virtual void findNext()=0;
    virtual void findPrevious()=0;
};

#endif // ISEARCHABLE_H
