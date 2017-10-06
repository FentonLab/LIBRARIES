/*   my_random.c  	*/
/*			*/ 

#include "librand.h"

#define DEB	0

long rint_my (double x) {
    return  (long)x == (long)(x+0.5) ? (long)x : (long)(x+0.5);
}    

double expdev (void)
   {
    double dum;

    do dum=drand48(); while (dum==0.0 || dum >=1.0);
    return -log(dum);
   }


void my_seed48 ( void )
{
        int     gettimeofday();
        void    srand48();
	double	drand48();
        struct  timeval tp;
        struct  timezone tzp;
	int 	i;
	unsigned int uintdummy;


	(void) gettimeofday ( &tp, &tzp );
	uintdummy = ( unsigned int ) tp.tv_usec; 

#if (DEB)
	printf ("\n uintdummy = %d", uintdummy);
#endif


        srand48( uintdummy );
	uintdummy = 100 + uintdummy % 100;


#if (DEB)
	printf ("\n uintdummy = %d", uintdummy);
	printf ("\n Strike a key ...");
	getchar();
#endif


	for(i = uintdummy; i--;)  { 


#if (DEB)
	    printf ("\n i = %d\t rand = %lf", i,  drand48() );
#endif


	    drand48();
	}


#if (DEB)
    	printf ("\n Strike a key ...");
	getchar();
#endif


}


double gammln(double xx)
{
        double x,tmp,ser;
        static double cof[6]={76.18009173,-86.50532033,24.01409822,
                -1.231739516,0.120858003e-2,-0.536382e-5};
        int j;

        x=xx-1.0;
        tmp=x+5.5;
        tmp -= (x+0.5)*log(tmp);
        ser=1.0;
        for (j=0;j<=5;j++) {
                x += 1.0;
                ser += cof[j]/x;
        }
        return -tmp+log(2.50662827465*ser);
}


double poidev(double xm)
/*	returns an integer (!!!) random number distrubuted with respect to Poisson p.d.f.	*/
/*	with an average equal to  xm;  modified from "Numerical Recipes in C" :  ran1 is chaged	*/
/*	to drand48(). IMPORTANT: drand48() works well being "warmed" by void my_seed (void);	*/

{
        static double sq,alxm,g,oldm=(-1.0);
        double em,t,y;

        if (xm < 12.0) {
                if (xm != oldm) {
                        oldm=xm;
                        g=exp(-xm);
                }
                em = -1;
                t=1.0;
                do {
                        em += 1.0;
                        t *= drand48();
                } while (t > g);
        } else {
                if (xm != oldm) {
                        oldm=xm;
                        sq=sqrt(2.0*xm);
                        alxm=log(xm);
                        g=xm*alxm-gammln(xm+1.0);
                }
                do {
                        do {
                                y=tan(M_PI*drand48());
                                em=sq*y+xm;
                        } while (em < 0.0);
                        em=floor(em);
                        t=0.9*(1.0+y*y)*exp(em*alxm-gammln(em+1.0)-g);
                } while (drand48() > t);
        }
        return em;
}

