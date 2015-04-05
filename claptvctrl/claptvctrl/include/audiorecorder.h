/**************************************************************

  Module:		Audio-recorder
  Version:	2013.04.18
  Author:		Nasir Ahmad (na200@kent.ac.uk)

  Description
  Records audio in real-time.

**************************************************************/


#pragma once
#include "utils.h"


/**
 Initialises the module.  Initialises the underlying audio subsystem, 
 allocates memory, etc.  Must be called prior to any ar calls.
 Returns 0 on success or an error number otherwise. 
*/
int arInitialise(void);


/**
 Starts recording.  Captures samples at a regular interval and puts them 
 in the program buffer
 Returns 0 on success or an error number otherwise. 
*/
int arStartRecording(void);


/**
 Stops recording.
*/
void arStopRecording(void);


/**
 Destruction code.  Releases resources. 
 Must be called at the end.
*/
void arFree(void);


/**
 Determines if the audio recorder is currently recording
 Returns TRUE/FALSE
*/
BOOL arIsRunning(void);


/**
 Locks the samples buffer to avoid race conditions
 Useful for multi-threading
*/
void arAcquireBuffer(void);


/**
 Unlocks the samples buffer so that other threads can access it
 Useful for multi-threading
*/
void arReleaseBuffer(void);


/**
 Read-only accessor to the samples buffer
 Returns a read-only pointer to the samples buffer
*/
const unsigned char* arBuffer(void);


/**
 Returns size of the samples buffer 
*/
const size_t arBufferSize(void);


