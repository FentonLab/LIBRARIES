#include <time.h>

void    seed_rand48()
{
        int     gettimeofday();
        void    srand48();
        struct  timeval tp;
        struct  timezone tzp;

        (void)gettimeofday(&tp,&tzp);
        srand48(tp.tv_usec);

        return;
}
