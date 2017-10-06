#include <math.h>
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

