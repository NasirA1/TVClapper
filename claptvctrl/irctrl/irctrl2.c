#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include "iguanaIR.h"
 
#define ARRAY_SIZE(x) ((sizeof x) / (sizeof *x))
#define IR_FAILURE	-1
#define IR_SUCCESS	0
#define COMMAND_COUNT	(6)
#define FALSE	(0)
#define TRUE	(1)

typedef unsigned char BOOL;


typedef enum tagcpCommand
{
	cpCmdUnknown,
	cpCmdPower,
	cpCmdVolUp,
	cpCmdVolDown,
	cpCmdChanUp,
	cpCmdChanDown
} cpCommand;

typedef struct tagcpCommandInfo {
    int id;
    char* name;
} cpCommandInfo;

typedef struct tagirPacket
{
	cpCommand commandId;
	unsigned int* signal;
	size_t length;
} irPacket;


int getFilenameForCommand(cpCommand command, char* const outFilename, const size_t len);
int loadIRSignal(cpCommand command, irPacket* const packet);
int loadIRData(void);
char* removeFileName(char* path);
int transmitPacket(unsigned int* packet, size_t size);
void printUsage(void);
int irTransmit(const cpCommand command);
void irFree(void);
char* cpId2str(const int id);
void sleep2(unsigned int microseconds);



irPacket commandPackets[COMMAND_COUNT];
cpCommandInfo cpCommands[] = {
    { cpCmdUnknown, 	"cpCmdUnknown" 	},
    { cpCmdPower, 		"cpCmdPower" 		},
    { cpCmdVolUp, 		"cpCmdVolUp" 		},
    { cpCmdVolDown, 	"cpCmdVolDown" 	},
    { cpCmdChanUp, 		"cpCmdChanUp" 	},
    { cpCmdChanDown,	"cpCmdChanDown" },
    { 0, 0 }
};


int main(int argc, char* argv[])
{	
	if(argc <= 1)
	{
		printUsage();
		exit(1); 
	}

	if(loadIRData() == IR_FAILURE)
	{
		printf("ERROR: Unable to initialise. Aborted!\n"); 
		exit(1);
	}
	
	const char* cmd = argv[1];
		
	if(!strcmp(cmd, "p"))
	{
		irTransmit(cpCmdPower);		
	}
	else if(!strcmp(cmd, "c+"))
	{
		irTransmit(cpCmdChanUp);		
	}
	else if(!strcmp(cmd, "c-"))
	{
		irTransmit(cpCmdChanDown);		
	}
	else if(!strcmp(cmd, "v+"))
	{
		irTransmit(cpCmdVolUp);		
	}
	else if(!strcmp(cmd, "v-"))
	{
		irTransmit(cpCmdVolDown);		
	}
	else
	{
		irFree();
		printUsage();
		exit(1);
	}


	irFree();
	return 0;
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



char* cpId2str(const int id)
{
	int i;
	for (i = 0; cpCommands[i].name; ++i)
	{
  	if (cpCommands[i].id == id) return cpCommands[i].name;
	}
	return "unknown";
}



int irTransmit(const cpCommand command)
{
	if(command == cpCmdUnknown) return IR_FAILURE;

	return transmitPacket(commandPackets[command].signal, commandPackets[command].length);
}



void irFree(void)
{
	int i;
	for(i = 0; i < COMMAND_COUNT; ++i)
	{
		free(commandPackets[i].signal);
	}
}


int transmitPacket(unsigned int* packet, size_t size)
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



void sleep2(unsigned int microseconds)
{
	clock_t goal = microseconds + clock();
	while (goal > clock());
}


char* removeFileName(char* path)
{
	int offset = strlen(path);
	while (offset && path[offset - 1] != '/') --offset;
	if (offset && path[offset - 1] == '/') --offset;
	path[offset] = 0;
	return path;
}



int loadIRData(void)
{
	unsigned int cmd;

	for(cmd = cpCmdPower; cmd < COMMAND_COUNT; ++cmd)
	{
		if(loadIRSignal(cmd, &commandPackets[cmd]) == IR_FAILURE) return IR_FAILURE;
	}
 
	return IR_SUCCESS;
}



int getFilenameForCommand(cpCommand command, char* const outFilename, const size_t len)
{
	char commandfile[10] = {0};

	sprintf(commandfile, "/%03d.ir", command);
	
	if((readlink("/proc/self/exe", outFilename, len - 11)) == -1)
	{
		perror("readlink");
		return IR_FAILURE;
	}

	removeFileName(outFilename);
	strcat(outFilename, commandfile);

	return IR_SUCCESS;
}



int loadIRSignal(cpCommand command, irPacket* const packet)
{
	if(!packet) return IR_FAILURE;
	if(command == cpCmdUnknown) return IR_FAILURE;
	
	memset(packet, 0, sizeof(irPacket));
	packet->commandId = command;

	char commandfilepath[1024] = {0};
	if(getFilenameForCommand(command, commandfilepath, 
		sizeof(commandfilepath)) == IR_FAILURE) return IR_FAILURE;

	//fprintf(stderr, "Loading IR data for command: %s from file: %03d.ir ...", 
	//	cpId2str(packet->commandId), command);

	FILE* fd = fopen(commandfilepath, "r");

	if(!fd)
	{
		perror("\nfopen");
		return IR_FAILURE;
	}

	char line[64] = {0};
	while((fgets(line, sizeof(line), fd)) != NULL) {
		char delims[] = "\t\n: ";
		char* token = NULL;
		token = strtok(line, delims);
		BOOL pulse = !strcmp(token, "pulse");
  	token = strtok(NULL, delims);
		packet->length++;
		packet->signal = (unsigned int*)realloc(packet->signal, packet->length * sizeof(unsigned int));
		packet->signal[packet->length - 1] = atoi(token) | (pulse? IG_PULSE_BIT: 0);
	}

	fclose(fd);
	
	//fprintf(stderr, "DONE\n");
	return IR_SUCCESS;
}


