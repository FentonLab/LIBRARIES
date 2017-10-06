#include	<stdio.h>
#include "libsize_types.h"

si1	*sscanfLine(si1 *dp, si1 *s)
{
	int	i = 0;

	*s = '\0';
	while (sscanf(dp, "%c", s + i) != EOF){
		dp++;
		if(*(s + i) == '\n'){
			*(s + i + 1) = '\0';
			return s;
		}
		++i;
	}
	*(s + i) = '\0';
	return s;
}

