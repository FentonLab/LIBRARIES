/*	returns a string containing the present line	*/

#include <stdio.h>

#define	MAX_CHARS	1000

char    *get_line(fp)
        FILE    *fp;
{
        static char line[MAX_CHARS];
        int     i=0,c;

        while((c = getc(fp)) != EOF){
                sprintf(line+i,"%c",c);
                i++;
                if(c == '\n')
                        return(line);
        }
        return(NULL);
}
