#include <stdlib.h>
#include <stdio.h>

char *get_line (FILE *fp, char *s) 
{
    int i=0;
    
    while (fscanf(fp, "%c", s+i) != EOF) {
	if (*(s+i) == '\n') {
	    *(s+i) = '\0';
	    return s;
	}
	i++;
    }
    return NULL;
}	    
