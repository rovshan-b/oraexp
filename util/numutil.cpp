#include "numutil.h"
#include <limits>

int NumUtil::increase(int &i)
{
    if(i == std::numeric_limits<int>::max()){
        i = 0;
    }else{
        ++i;
    }

    return i;
}

NumUtil::NumUtil()
{
}
