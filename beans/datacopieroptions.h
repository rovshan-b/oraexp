#ifndef DATACOPIEROPTIONS_H
#define DATACOPIEROPTIONS_H

#include "dataoperationoptions.h"

class DataCopierOptions : public DataOperationOptions
{
public:
    DataCopierOptions();

    bool truncate;
};

#endif // DATACOPIEROPTIONS_H
