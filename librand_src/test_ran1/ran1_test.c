#include <stdio.h>
#include <stdlib.h>
#include <librand.h>

#define N 10000


int main ()
{
	FILE *out, *avg;
	int i, *i_dum;

	double 
	    s1, s2, ddummy,
	    mean, variance;
	
        out = fopen ("output", "w");
        avg = fopen ("average", "w");

	*i_dum = -7;

	s1 = s2 = 0.0;
	for (i=0; i < N; i++) {
		ddummy = ran1(i_dum);
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
	return (0);
}





