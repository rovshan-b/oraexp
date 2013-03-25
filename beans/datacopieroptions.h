#ifndef DATACOPIEROPTIONS_H
#define DATACOPIEROPTIONS_H

#include "dataoperationoptions.h"

class DataCopierOptions : public DataOperationOptions
{
public:
    enum TruncateMode
    {
        Truncate,
        TruncateAndCascadeConstraints,
        DoNotTruncate
    };

    DataCopierOptions();

    bool truncate;
};

#endif // DATACOPIEROPTIONS_H
