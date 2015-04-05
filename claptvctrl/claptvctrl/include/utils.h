/**************************************************************

  Module:		Utilities
  Version:	2013.04.18
  Author:		Nasir Ahmad (na200@kent.ac.uk)

  Description
	Contains various useful utility functions

**************************************************************/


#pragma once
#include <stdint.h>
#include <sys/time.h>


/**
 Returns size of the given array provided that it is known
 at compile-time.
 Do NOT use this function with malloc'ed arrays
*/
#define ARRAY_SIZE(x) ((sizeof x) / (sizeof *x))


/**
 Efficient way of representing the boolean data type (takes up 1 byte only)
*/
#define FALSE	(0)
#define TRUE	(1)
typedef unsigned char BOOL;


/**
 Removes filename from the given path
 e.g. ~/Desktop/Documents/abc.txt becomes ~/Desktop/Documents

 Warning: Modifies the given string
 Returns a pointer to the original string passed
*/
char* removeFileName(char* path);


/**
 Returns the difference between the two given timespec's
 Returns the difference in milliseconds
*/
int64_t timeDiff(struct timeval* tCurr, struct timeval* tPrev);



