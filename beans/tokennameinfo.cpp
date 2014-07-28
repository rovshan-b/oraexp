#include "tokennameinfo.h"

TokenNameInfo::TokenNameInfo()
{
}

bool TokenNameInfo::isEmpty() const
{
    return parts.isEmpty();
}

QString TokenNameInfo::toString() const
{
    QString result;

    for(int i=0; i<parts.size(); ++i){
        result.append(parts[i]);

        if(i != parts.size()-1){
            result.append('.');
        }
    }

    return result;
}
