#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include "utils.h"
#include "commandparser.h"


//#define _DEBUG

#ifdef _DEBUG
#define DEBUGPRINT_BUFFER()	fprintf(stderr, "|%d|%d|%d|%d|%d|\n", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4])
#else
#define DEBUGPRINT_BUFFER()
#endif


static BOOL buffer[COMMAND_BUFFER_SIZE];
static size_t timeSlot;
static struct timeval tStart;
static cpCommand lastCommand;

cpCommandInfo cpCommands[] = {
    { cpCmdUnknown, 	"cpCmdUnknown" 	},
    { cpCmdPower, 		"cpCmdPower" 		},
    { cpCmdVolUp, 		"cpCmdVolUp" 		},
    { cpCmdVolDown, 	"cpCmdVolDown" 	},
    { cpCmdChanUp, 		"cpCmdChanUp" 	},
    { cpCmdChanDown,	"cpCmdChanDown" },
    { 0, 0 }
};


/*
	Command Definitions

	1: Clap
	0: 100-200ms Silence
*/
cpCommandPattern commands[] = { 
	{ cpCmdPower, 	{{1,0,1,0,1}, {1,0,1,0,1}} }, 
	{ cpCmdVolUp, 	{{1,1,0,1,0}, {1,1,0,0,1}} }, 
	{ cpCmdVolDown, {{1,0,0,1,1}, {1,0,1,1,0}} },
	{ cpCmdChanUp, 	{{1,0,0,0,1}, {1,0,0,0,1}} }, 
	{ cpCmdChanDown,{{1,0,1,0,0}, {1,0,0,1,0}} },
};

/* 'Last command' can be repeated via a single clap */
BOOL repeatLastCmdPattern[] = {1,0,0,0,0};


BOOL buffer_empty(void);
char* cpId2str(const int id);
void reset(void);
void registerCommandStart(void);




void cpInitialise(void)
{
	reset();
	lastCommand = cpCmdUnknown;
}



void cpSignal(int64_t elapsedSinceLastSignal)
{
	if( elapsedSinceLastSignal > (COMMAND_BUFFER_SIZE * 200) )  //1 second time window for each command
	{
		registerCommandStart();
		DEBUGPRINT_BUFFER();
		return;
	}

	if(elapsedSinceLastSignal >= 0 && elapsedSinceLastSignal <= 200)
	{
		timeSlot++;
	}
	else if(elapsedSinceLastSignal > 200 && elapsedSinceLastSignal <= 400)
	{
		timeSlot += 2;
	}
	else if(elapsedSinceLastSignal > 400 && elapsedSinceLastSignal <= 600)
	{
		timeSlot += 3;
	}
	else /* anything > 600ms */
	{
		timeSlot += 4;
	}
	
	if(timeSlot > COMMAND_BUFFER_SIZE)
	{
		registerCommandStart();
	}
	else
	{
		buffer[timeSlot] = TRUE;
	}

	DEBUGPRINT_BUFFER();
}



cpCommand cpRecogniseCommand(void)
{
	//Basic case: Nothing was captured
	if(buffer_empty()) return cpCmdUnknown;

	struct timeval tCurr;
	gettimeofday(&tCurr, 0);
	int64_t elapsedMs = timeDiff(&tCurr, &tStart);
	
	if(elapsedMs >= (COMMAND_BUFFER_SIZE * 200))
	{
		//Matched 'Last Command' (Single clap) pattern 
		if(!memcmp(buffer, repeatLastCmdPattern, COMMAND_BUFFER_SIZE) 
			&& lastCommand != cpCmdUnknown)
		{
			reset(); 
			return lastCommand;
		}

		int i, j;
		for(i = 0; i < ARRAY_SIZE(commands); i++)
		{
			for(j = 0; j < ARRAY_SIZE(commands[i].patterns); j++)
			{
				//We've found a match!
				if(!memcmp(buffer, commands[i].patterns[j], COMMAND_BUFFER_SIZE))
				{
					reset();
					
					//Remember last command (so that it can be repeated via a single clap)
					//Exclude cpCmdPower command from this rule
					if(commands[i].commandId == cpCmdPower) lastCommand = cpCmdUnknown;
					else lastCommand = commands[i].commandId;					
	
					return commands[i].commandId;
				}
			}
		}

		lastCommand = cpCmdUnknown;
	}
	
	return cpCmdUnknown;
}



BOOL buffer_empty(void)
{
	int i;

	for(i = 0; i < COMMAND_BUFFER_SIZE; ++i)
	{
		if(buffer[i]) return FALSE;
	}

	return TRUE;
}



char* cpId2str(const int id)
{
	int i;
	for (i = 0; cpCommands[i].name; ++i)
	{
  	if (cpCommands[i].id == id) return cpCommands[i].name;
	}
	return "unknown";
}



void reset(void)
{
	//fprintf(stdin, "RESET\n");
	timeSlot = 0;
	memset(buffer, 0, COMMAND_BUFFER_SIZE);
	memset(&tStart, 0, sizeof(struct timeval));	
}



void registerCommandStart(void)
{
	reset();
	buffer[0] = TRUE;
	gettimeofday(&tStart, 0);
}


