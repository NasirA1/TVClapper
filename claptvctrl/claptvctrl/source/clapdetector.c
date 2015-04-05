#include <string.h>
#include <stdlib.h>
#include "utils.h"
#include <stdio.h>
#include "clapdetector.h"



double* energyBuffer;  		//local 1 second energy history buffer
int currentPos; 					//current slot of the history buffer
BOOL shift;     					//determines whether to shift elements of energies array
double sensitivity;  			//algorithm sensitivity
struct timeval tPrev;
unsigned int minGapMs;		//minimum time-gap between consecutive claps (ms)




void cdInitialise(const double sensitivity_, const unsigned int minGapMs_)
{
	sensitivity = sensitivity_;
	minGapMs = minGapMs_;
	currentPos = ENERGY_BUFFER_SIZE - 1;
	shift = FALSE;

	energyBuffer = (double*)malloc(ENERGY_BUFFER_SIZE * sizeof(double));

	if(!energyBuffer)
	{
		fprintf(stderr, "cdInitialise: Unable to allocate memory!\n");
		exit(1);
	}

	memset(energyBuffer, 0, ENERGY_BUFFER_SIZE * sizeof(double));
}


void cdFree(void)
{
	free(energyBuffer);
}


void cdSetSensitivity(const double value)
{
	sensitivity = value;
}


void cdSetMinGapMs(const unsigned int value)
{
	minGapMs = value;
}


void cdDetectClap(
	const unsigned char* const bytes, 
	const size_t length,  
	void (*Callback)(double energy, int64_t elapsedSinceLastClap)
)
{
	int i;
	double instantEnergy = 0.0;
	double averageEnergy = 0.0;
	double sum = 0.0;

	//Calculate instant energy of sample buffer
	//ONLY handles mono audio 16bit little endian format!
	for (i = 1; i < length; i += 2)
	{
		short sample_s = ((bytes[i] & 0xFF) << 8) | (bytes[i - 1] & 0xFF);
		sample_s *= SCALE_FACTOR;
		instantEnergy += ((double)sample_s * (double)sample_s);
	}

	//compute average local energy
	for (i = 0; i < ENERGY_BUFFER_SIZE; i++) {
		sum += energyBuffer[i];
	}
	averageEnergy = sum / (double)ENERGY_BUFFER_SIZE;

	//Update energy buffer
	energyBuffer[currentPos] = instantEnergy;

	//if history buffer full, shift the array 1 slot down
	if (shift) {
		//Remove the oldest energy to make room for new energy on top
		for (i = ENERGY_BUFFER_SIZE - 1; i > 0; i--) {
			energyBuffer[i] = energyBuffer[i - 1];
		}
		currentPos = 0;
	} else {
		currentPos--;
	}
	if (currentPos < 0) {
		currentPos = 0;
		shift = TRUE;
	}

	//determine clap
	if(instantEnergy > (sensitivity * averageEnergy)) 
	{
		struct timeval tCurr;
	 	gettimeofday(&tCurr, 0);
		int64_t elapsedMs = timeDiff(&tCurr, &tPrev);
		if(elapsedMs >= minGapMs && Callback) Callback(instantEnergy, elapsedMs);
		memcpy(&tPrev, &tCurr, sizeof(struct timeval));
	}
}


