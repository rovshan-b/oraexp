#ifndef IDBTREEITEMCHECKSTATEPROVIDER_H
#define IDBTREEITEMCHECKSTATEPROVIDER_H

class DbTreeItem;

class IDbTreeItemCheckStateProvider
{
public:
    virtual ~IDbTreeItemCheckStateProvider(){}

    virtual bool isCheckable(DbTreeItem *item)=0;
};

#endif // IDBTREEITEMCHECKSTATEPROVIDER_H
