#include <math.h>
#include <stdlib.h>

double expdev (void)
   {
    double dum;

    do 	dum=drand48(); while (dum==0.0 || dum >=1.0);
    return -log(dum);
   }

