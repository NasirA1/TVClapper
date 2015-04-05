#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "iguanaIR.h"
#include "irtransmitter.h"
#include "utils.h"




//Variables
irPacket commandPackets[COMMAND_COUNT];



//Helpers
int transmitPacket(unsigned int* packet, size_t size);
int transmitPacketReceiveResponse(unsigned int* packet, size_t size);
int getFilenameForCommand(cpCommand command, char* const outFilename, const size_t len);
int loadIRSignal(cpCommand command, irPacket* const packet);
int loadIRData(void);


//Interface functions

int irInitialise(void)
{
	int rc = IR_FAILURE;

	rc = loadIRData();

	if(rc == IR_FAILURE) return IR_FAILURE;

	/* connect to a device, in this case the first device found by igdaemon */
	PIPE_PTR conn = iguanaConnect("0");
	
	if (conn == INVALID_PIPE)
	{
		rc = IR_FAILURE;
		perror("iguanaConnect failed");
	}
	else
	{
		iguanaPacket req, resp;

		/* check the version just to demonstrate the server/client protocol */
		req = iguanaCreateRequest(IG_DEV_GETVERSION, 0, NULL);
		if (!iguanaWriteRequest(req, conn))
		{
			rc = IR_FAILURE;
			perror("iguanaWriteRequest failed");
		}
		else
		{
			/* wait up to 1000 milliseconds for a response */
			resp = iguanaReadResponse(conn, 1000);
			if (iguanaResponseIsError(resp))
			{
				rc = IR_FAILURE;
				perror("iguanaReadResponse errored");
			}
			else
			{
				/* the get version request returns 2 version bytes */
				unsigned int len;
				unsigned char *buffer = iguanaRemoveData(resp, &len);
				printf("Successfully connected to Iguana IR Transceiver [Firmware version 0x%02x]\n", *((short*)buffer));
				rc = IR_SUCCESS;
				free(buffer);
			}
			iguanaFreePacket(resp);
		}
		
		iguanaFreePacket(req);
	}

	return rc;
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


/********************** Helpers Functions *********************************/


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
	if(getFilenameForCommand(command, commandfilepath, sizeof(commandfilepath)) == IR_FAILURE) return IR_FAILURE;

	fprintf(stderr, "Loading IR data for command: %s from file: %03d.ir ...", 
		cpId2str(packet->commandId), command);

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
	
	fprintf(stderr, "DONE\n");
	return IR_SUCCESS;
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
		else
		{
			rc = IR_SUCCESS;
		}

		/* because we did not dynamically allocate the buffer we need
		to remove it before freeing the packet it was added to. */
		iguanaRemoveData(req, NULL);
		iguanaFreePacket(req);
	}

	return rc;
}



int transmitPacketReceiveResponse(unsigned int* packet, size_t size)
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
		iguanaPacket req, resp;
		req = iguanaCreateRequest(IG_DEV_SEND, sizeof(unsigned int) * size, packet);
		
		if (!iguanaWriteRequest(req, conn))
		{
			perror("iguanaWriteRequest failed");
		}
		else
		{
			resp = iguanaReadResponse(conn, 1000);
			if (iguanaResponseIsError(resp))
			{
				perror("iguanaReadResponse errored");
			}
			else
			{
				/* send just gives back success or failure */
				rc = IR_SUCCESS;
			}
			
			iguanaFreePacket(resp);
		}

		/* because we did not dynamically allocate the buffer we need
		to remove it before freeing the packet it was added to. */
		iguanaRemoveData(req, NULL);
		iguanaFreePacket(req);
	}

	return rc;
}


