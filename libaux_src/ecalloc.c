#include <stdio.h>
#include <stdlib.h>

void	*ecalloc(size_t num_elements, size_t size){
	void	*ptr;

	ptr = calloc(num_elements, size);
	if(ptr == NULL){
		fprintf(stderr,"Could not calloc %d elements of size %d bytes. Exiting program.\n", num_elements, size);
		exit(-1);
	}
	return(ptr);
}

