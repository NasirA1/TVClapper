/**************************************************************

  Module:		Clap-detector
  Version:	2013.04.18
  Author:		Nasir Ahmad (na200@kent.ac.uk)

  Description
  Detects claps.  Employs a simple statistical analysis of the
  samples passed (in the time-domain).  The algorithm is therefore
  'colour-blind' as it disregards frequencies.  In other words, 
  it cannot tell the difference between a clap and for example
  a loud click (or any type of other sudden noise)

  IMPORTANT
  Only works with 16bit little endian mono audio data

**************************************************************/


#pragma once
#include <stddef.h>
#include <stdint.h>
#include "settings.h"

//Size of the energy buffer
//Holds 1 second's worth of data
#define ENERGY_BUFFER_SIZE	43


/**
 Initialises the module
*/
void cdInitialise(
	const double sensitivity_,
	const unsigned int minGapMs_
);


/**
 Performs clean-up operations.
 Releases resources.
*/
void cdFree(void);


/**
 Performs 'clap-detection' analysis on the audio data passed
 and calls the callback function passed in the 3rd argument
 when a clap is detected.

 WARNING: ONLY handles mono audio 16bit little endian format!
*/
void cdDetectClap(
	const unsigned char* const bytes,
	const size_t length,
	void (*Callback)(double energy, int64_t elapsedSinceLastClap)
);


