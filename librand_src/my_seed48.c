#include "librand.h"

void my_seed48 ( void )
{
        int     gettimeofday();
        void    srand48();
        struct  timeval tp;
        struct  timezone tzp;
	int 	i;
	unsigned int uintdummy;


	(void) gettimeofday ( &tp, &tzp );
	uintdummy = ( unsigned int ) tp.tv_usec; 


        srand48( uintdummy );
	uintdummy = 100 + uintdummy % 100;

	for(i = uintdummy; i--;)     drand48();
}
