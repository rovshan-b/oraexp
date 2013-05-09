#ifndef IDATAIMPORTLISTENER_H
#define IDATAIMPORTLISTENER_H

class IDataImportListener
{
public:
    virtual void headerAvailable(const QStringList &headerTitles) = 0;
    virtual void rowAvailable(const QStringList &values) = 0;
};

#endif // IDATAIMPORTLISTENER_H
