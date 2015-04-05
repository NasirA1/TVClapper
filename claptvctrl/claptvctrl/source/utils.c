#include <string.h>
#include "utils.h"



char* removeFileName(char* path)
{
	int offset = strlen(path);
	while (offset && path[offset - 1] != '/') --offset;
	if (offset && path[offset - 1] == '/') --offset;
	path[offset] = 0;
	return path;
}


int64_t timeDiff(struct timeval* tCurr, struct timeval* tPrev)
{
  long seconds  = tCurr->tv_sec  - tPrev->tv_sec;
  long useconds = tCurr->tv_usec - tPrev->tv_usec;
  int64_t elapsedMs = ((seconds) * 1000 + useconds/1000.0) + 0.5;
	return elapsedMs; 	
}


