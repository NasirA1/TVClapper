#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "audiorecorder.h"
#include "clapdetector.h"
#include "commandparser.h"
#include "irtransmitter.h"



/**
 Callback function
 Called by the clap detector whenever a clap is detected
 First detected 'clap' is always ignored as it is 'false' due to absence
 of energy in the signal buffer at the beginning
*/
void clapDetectedCallback(double energy, int64_t elapsedSinceLastClap)
{
	static unsigned long count = 0;

	if(count)
	{
		//fprintf(stderr, "%ld - %s (Strength: %f) - Elapsed=%" PRId64 "\n", count, "Clap!", energy, elapsedSinceLastClap);
		cpSignal(elapsedSinceLastClap);
	}

	count++;
}


/**
 Main entry point
*/
int main(void)
{
	int rc;

	//initialise audio recorder
	rc = arInitialise();
	if(rc)
	{
		fprintf(stderr, "Fatal error: Audio could not be initialised <%d>\n", rc);
		arFree();
		exit(1);
	}

	//initialise IR transmitter
	rc = irInitialise();
	if(rc == IR_FAILURE)
	{
		fprintf(stderr, "Fatal error: IR transmitter could not be initialised <%d>\n", rc);
		//irFree();
		//arFree();
		//exit(1);
	}

	//initialise clap detector
	cdInitialise(CD_SENSITIVITY, CD_MINGAPMS);

	//start recording
	rc = arStartRecording();

	//application loop
	//Capture samples, detect claps, recognise clap patterns
	//and send recognised commands to the TV via infrared
	while(arIsRunning())
	{
		cdDetectClap(arBuffer(), arBufferSize(), clapDetectedCallback);

		cpCommand cmd = cpRecogniseCommand();

		if(cmd != cpCmdUnknown)
		{
			int i; int count;
			//transmit volume commands 'count' no. times
			if(cmd == cpCmdVolUp || cmd == cpCmdVolDown) count = 5;
			else count = 1;
	
			for(i = 0; i < count; ++i)
			{
				rc = irTransmit(cmd);
				fprintf(stderr, "Transmit command %s... rc=%d\n", cpId2str(cmd), rc);
			}
		}
	}

	//Clean up
	irFree();
	cdFree();
	arFree();

  return 0;
}


