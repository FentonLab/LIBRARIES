#include <stdio.h>
#include <stdlib.h>

FILE	*efopen(char *f, char *mode){
	FILE	*fp;

	fp = fopen(f,mode);
	if(fp == NULL){
		fprintf(stderr,"Could not open file: %s. Exiting program.\n",f);
		exit(-1);
	}
	return(fp);
}

