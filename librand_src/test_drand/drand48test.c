#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

#define N 10000

void my_seed48 ( void );

void main ()
{
	FILE *out, *avg;
	int i;
	double 
	    s1, s2, ddummy,
	    mean, variance;
	
        out = fopen ("output", "w");
        avg = fopen ("average", "w");

	my_seed48();			
	s1 = s2 = 0.0;
	for (i=0; i < N; i++) {
		ddummy = drand48();
		if ( ddummy > 1.0 || ddummy < 0.0 ) printf ("\n ! ddummy = %e ", ddummy );
	        s1 += ddummy;
		s2 += ddummy*ddummy;
	        fprintf (out,"%e\n", ddummy  );
	        if ( i > 0 ) fprintf (avg,"%e\n", s1 / (i + 1.0) ) ;
	}
	mean = s1/N;
	variance = s2/N - mean*mean;
	printf ("\n mean = %7.4f\n var = %7.4f\n", mean, variance); 
	
	
	fclose (out);
	fclose (avg);

}

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
	printf ("\n uintdummy = %d", uintdummy);
	getchar();
        srand48( uintdummy );
	uintdummy = 100 + uintdummy % 100;
	printf ("\n uintdummy = %d", uintdummy);
	 
	for(i = uintdummy; i--;)
	    drand48();
}







