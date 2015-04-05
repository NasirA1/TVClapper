#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "iguanaIR.h"
 
#define ARRAY_SIZE(x) ((sizeof x) / (sizeof *x))
#define IR_FAILURE	-1
#define IR_SUCCESS	0

		unsigned int power[] = {		
			917 | IG_PULSE_BIT,
			832,
			896 | IG_PULSE_BIT,
			810,
			1770 | IG_PULSE_BIT,
			832,
			896 | IG_PULSE_BIT,
			832,
			896 | IG_PULSE_BIT,
			832,
			896 | IG_PULSE_BIT,
			1706,
			1749 | IG_PULSE_BIT,
			853,
			896 | IG_PULSE_BIT,
			1706,
			896 | IG_PULSE_BIT,
			810,
			1749 | IG_PULSE_BIT,
			853,
			896 | IG_PULSE_BIT,
			21845,
			70997 | IG_PULSE_BIT,
			917,
			832 | IG_PULSE_BIT,
			896,
			832 | IG_PULSE_BIT,
			1749,
			853 | IG_PULSE_BIT,
			896,
			832 | IG_PULSE_BIT,
			896,
			832 | IG_PULSE_BIT,
			874,
			1728 | IG_PULSE_BIT,
			1792,
			832 | IG_PULSE_BIT,
			896,
			1706 | IG_PULSE_BIT,
			896,
			832 | IG_PULSE_BIT,
			1770,
			810 | IG_PULSE_BIT,
			917,
			43690 | IG_PULSE_BIT,
			68266,
			917 | IG_PULSE_BIT,
			832,
			896 | IG_PULSE_BIT,
			832,
			1792 | IG_PULSE_BIT,
			832,
			896 | IG_PULSE_BIT,
			832,
			896 | IG_PULSE_BIT,
			832,
			896 | IG_PULSE_BIT,
			1706,
			1770 | IG_PULSE_BIT,
			832,
			896 | IG_PULSE_BIT,
			1685,
			917 | IG_PULSE_BIT,
			832,
			1792 | IG_PULSE_BIT,
			832,
			896 | IG_PULSE_BIT,
			65536
		};


unsigned int volup[] = {
	874 | IG_PULSE_BIT,
	853,
	874 | IG_PULSE_BIT,
	853,
	1749 | IG_PULSE_BIT,
	853,
	874 | IG_PULSE_BIT,
	853,
	874 | IG_PULSE_BIT,
	853,
	853 | IG_PULSE_BIT,
	1749,
	1749 | IG_PULSE_BIT,
	1728,
	1749 | IG_PULSE_BIT,
	853,
	874 | IG_PULSE_BIT,
	853,
	874 | IG_PULSE_BIT,
	853,
	853 | IG_PULSE_BIT,
	90240,
	896 | IG_PULSE_BIT,
	853,
	853 | IG_PULSE_BIT,
	853,
	1749 | IG_PULSE_BIT,
	853,
	874 | IG_PULSE_BIT,
	853,
	853 | IG_PULSE_BIT,
	853,
	874 | IG_PULSE_BIT,
	1728,
	1749 | IG_PULSE_BIT,
	1728,
	1749 | IG_PULSE_BIT,
	853,
	874 | IG_PULSE_BIT,
	853,
	874 | IG_PULSE_BIT,
	853,
	874 | IG_PULSE_BIT,
	131072, 
	874 | IG_PULSE_BIT,
	853,
	1749 | IG_PULSE_BIT,
	853,
	874 | IG_PULSE_BIT,
	853,
	874 | IG_PULSE_BIT,
	853,
	874 | IG_PULSE_BIT,
	853,
	874 | IG_PULSE_BIT,
	1749,
	1749 | IG_PULSE_BIT,
	1749,
	1749 | IG_PULSE_BIT,
	853,
	874 | IG_PULSE_BIT,
	853,
	874 | IG_PULSE_BIT,
	853,
	874 | IG_PULSE_BIT,
	92842
};

unsigned int voldn[] = {
	917 | IG_PULSE_BIT,
	832,
	896 | IG_PULSE_BIT,
	832,
	1770 | IG_PULSE_BIT,
	832,
	896 | IG_PULSE_BIT,
	810,
	917 | IG_PULSE_BIT,
	832,
	896 | IG_PULSE_BIT,
	1706,
	1813 | IG_PULSE_BIT,
	1685,
	1770 | IG_PULSE_BIT,
	832,
	896 | IG_PULSE_BIT,
	810,
	917 | IG_PULSE_BIT,
	1685,
	917 | IG_PULSE_BIT,
	89215,
	917 | IG_PULSE_BIT,
	832,
	896 | IG_PULSE_BIT,
	832,
	1770 | IG_PULSE_BIT,
	832,
	874 | IG_PULSE_BIT,
	853,
	917 | IG_PULSE_BIT,
	789,
	896 | IG_PULSE_BIT,
	1706,
	1792 | IG_PULSE_BIT,
	1685,
	1813 | IG_PULSE_BIT,
	810,
	896 | IG_PULSE_BIT,
	832,
	896 | IG_PULSE_BIT,
	1706,
	896 | IG_PULSE_BIT,
	131072, 
	896 | IG_PULSE_BIT,
	832,
	1749 | IG_PULSE_BIT,
	853,
	896 | IG_PULSE_BIT,
	832,
	896 | IG_PULSE_BIT,
	832,
	938 | IG_PULSE_BIT,
	789,
	896 | IG_PULSE_BIT,
	1706,
	1749 | IG_PULSE_BIT,
	1728,
	1749 | IG_PULSE_BIT,
	853,
	896 | IG_PULSE_BIT,
	832,
	896 | IG_PULSE_BIT,
	1706,
	896 | IG_PULSE_BIT,
	92842
};

unsigned int chandn[] = {
	917 | IG_PULSE_BIT,
	832,
	896 | IG_PULSE_BIT,
	832,
	1770 | IG_PULSE_BIT,
	832,
	896 | IG_PULSE_BIT,
	832,
	896 | IG_PULSE_BIT,
	810,
	874 | IG_PULSE_BIT,
	1685,
	853 | IG_PULSE_BIT,
	853,
	1749 | IG_PULSE_BIT,
	810,
	896 | IG_PULSE_BIT,
	832,
	896 | IG_PULSE_BIT,
	832,
	896 | IG_PULSE_BIT,
	1706,
	896 | IG_PULSE_BIT,
	89130,
	917 | IG_PULSE_BIT,
	810,
	896 | IG_PULSE_BIT,
	832,
	1770 | IG_PULSE_BIT,
	789,
	917 | IG_PULSE_BIT,
	832,
	874 | IG_PULSE_BIT,
	853,
	896 | IG_PULSE_BIT,
	1685,
	874 | IG_PULSE_BIT,
	853,
	1749 | IG_PULSE_BIT,
	853,
	896 | IG_PULSE_BIT,
	832,
	853 | IG_PULSE_BIT,
	853,
	896 | IG_PULSE_BIT,
	1706,
	896 | IG_PULSE_BIT,
	65984,
	917 | IG_PULSE_BIT,
	832,
	1749 | IG_PULSE_BIT,
	853,
	874 | IG_PULSE_BIT,
	853,
	896 | IG_PULSE_BIT,
	832,
	896 | IG_PULSE_BIT,
	810,
	917 | IG_PULSE_BIT,
	1706,
	896 | IG_PULSE_BIT,
	832,
	1749 | IG_PULSE_BIT,
	853,
	896 | IG_PULSE_BIT,
	832,
	896 | IG_PULSE_BIT,
	832,
	896 | IG_PULSE_BIT,
	1706,
	896 | IG_PULSE_BIT,
	89258,
	896 | IG_PULSE_BIT,
	853,
	1749 | IG_PULSE_BIT,
	853,
	896 | IG_PULSE_BIT,
	832,
	896 | IG_PULSE_BIT,
	810,
	874 | IG_PULSE_BIT,
	810,
	874 | IG_PULSE_BIT,
	1685,
	896 | IG_PULSE_BIT,
	810,
	1749 | IG_PULSE_BIT,
	874,
	874 | IG_PULSE_BIT,
	853,
	938 | IG_PULSE_BIT,
	832,
	917 | IG_PULSE_BIT,
	1728,
	896 | IG_PULSE_BIT,
	87381,
};

unsigned int chanup[] = {
	896 | IG_PULSE_BIT,
	853,
	1792 | IG_PULSE_BIT,
	810,
	917 | IG_PULSE_BIT,
	832,
	853 | IG_PULSE_BIT,
	832,
	917 | IG_PULSE_BIT,
	810,
	896 | IG_PULSE_BIT,
	1706,
	896 | IG_PULSE_BIT,
	810,
	1749 | IG_PULSE_BIT,
	853,
	896 | IG_PULSE_BIT,
	832,
	896 | IG_PULSE_BIT,
	832,
	896 | IG_PULSE_BIT,
	832,
	896 | IG_PULSE_BIT,
	92842,
	917 | IG_PULSE_BIT,
	832,
	1749 | IG_PULSE_BIT,
	853,
	896 | IG_PULSE_BIT,
	832,
	874 | IG_PULSE_BIT,
	853,
	896 | IG_PULSE_BIT,
	832,
	896 | IG_PULSE_BIT,
	1685,
	917 | IG_PULSE_BIT,
	832,
	1770 | IG_PULSE_BIT,
	832,
	874 | IG_PULSE_BIT,
	853,
	896 | IG_PULSE_BIT,
	832,
	896 | IG_PULSE_BIT,
	832,
	874 | IG_PULSE_BIT,
	133184,
	917 | IG_PULSE_BIT,
	832,
	896 | IG_PULSE_BIT,
	832,
	1813 | IG_PULSE_BIT,
	810,
	874 | IG_PULSE_BIT,
	853,
	896 | IG_PULSE_BIT,
	810,
	896 | IG_PULSE_BIT,
	1706,
	896 | IG_PULSE_BIT,
	832,
	1770 | IG_PULSE_BIT,
	832,
	896 | IG_PULSE_BIT,
	832,
	896 | IG_PULSE_BIT,
	832,
	896 | IG_PULSE_BIT,
	832,
	896 | IG_PULSE_BIT,
	92842,
	874 | IG_PULSE_BIT,
	832,
	917 | IG_PULSE_BIT,
	832,
	1792 | IG_PULSE_BIT,
	810,
	874 | IG_PULSE_BIT,
	853,
	896 | IG_PULSE_BIT,
	832,
	896 | IG_PULSE_BIT,
	1706,
	874 | IG_PULSE_BIT,
	853,
	1792 | IG_PULSE_BIT,
	810,
	938 | IG_PULSE_BIT,
	789,
	896 | IG_PULSE_BIT,
	832,
	896 | IG_PULSE_BIT,
	832,
	896 | IG_PULSE_BIT,
	87381,
};




void sleep(unsigned int microseconds)
{
	clock_t goal = microseconds + clock();
	while (goal > clock());
}

void sleep2(unsigned int milliseconds)
{
	clock_t goal = (milliseconds * 1000) + clock();
	while (goal > clock());
}

int sendIRSignal(unsigned int* packet, size_t size)
{
	int rc = IR_FAILURE;

	PIPE_PTR conn = iguanaConnect("0");
	
	if (conn == INVALID_PIPE)
	{
		rc = IR_FAILURE;
		perror("iguanaConnect failed");
	}
	else
	{
		iguanaPacket req;
		req = iguanaCreateRequest(IG_DEV_SEND, sizeof(unsigned int) * size, packet);
		
		if (!iguanaWriteRequest(req, conn))
		{
			perror("iguanaWriteRequest failed");
		}

		/* because we did not dynamically allocate the buffer we need
		to remove it before freeing the packet it was added to. */
		iguanaRemoveData(req, NULL);
		iguanaFreePacket(req);
	}

	return rc;
}


void printUsage(void)
{
	printf("Usage: irctrl [command]\n"
		"and the commands are:\n"
		"\tPower:         p\n"
		"\tChannel Up:    c+\n"
		"\tChannel Down:  c-\n"
		"\tVolume Up:     v+\n"
		"\tVolume Down:   v-\n"
	);
}


int main(int argc, char* argv[])
{	
	if(argc <= 1)
	{
		printUsage();
		exit(1); 
	}

	
	const char* cmd = argv[1];
	
	if(!strcmp(cmd, "p"))
	{
		sendIRSignal(power, ARRAY_SIZE(power));		
	}
	else if(!strcmp(cmd, "c+"))
	{
		sendIRSignal(chanup, ARRAY_SIZE(chanup));
	}
	else if(!strcmp(cmd, "c-"))
	{
		sendIRSignal(chandn, ARRAY_SIZE(chandn));
	}
	else if(!strcmp(cmd, "v+"))
	{
		sendIRSignal(volup, ARRAY_SIZE(volup));
	}
	else if(!strcmp(cmd, "v-"))
	{
		sendIRSignal(voldn, ARRAY_SIZE(voldn));
	}
	else
	{
		printUsage();
		exit(1);
	}

	return 0;
}


