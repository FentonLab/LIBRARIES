
#include "libsess_anal.h"
#include "libsize_types.h"

si4     rec_time_sort(a, b)
RECORD  *a, *b;
{
        if (a->time_stamp > b->time_stamp)
                return(1);
        if (a->time_stamp < b->time_stamp)
                return(-1);
        return(0);
}
