/*	check_env.c		*/

#include <stdlib.h>
#include <stdio.h>

#define         SHOW_DIRS   printf("\n\nHERE IS YOUR 'DIR' ENVIRONMENT\n\n"); system("env | grep DIR\n");

void	check_env (char *name)
{
	if(getenv(name) == NULL){
		(void)fprintf(stderr,"%s is not defined\n", name);
		SHOW_DIRS
		instruct ();
	}
}





