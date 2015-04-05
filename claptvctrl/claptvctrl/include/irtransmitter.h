/**************************************************************

  Module:		IR Transmitter
  Version:	2013.04.18
  Author:		Nasir Ahmad (na200@kent.ac.uk)

  Description
	Transmits remote-control commands to the TV via infrared
  Infrared signal data is loaded from files which must reside
  in the application directory.

**************************************************************/


#pragma once
#include "commandparser.h"

#define IR_FAILURE	-1
#define IR_SUCCESS	0


/**
 An infrared command packet comprises of command id, the infrared
 signal of the command and size of the signal
*/
typedef struct tagirPacket
{
	cpCommand commandId;
	unsigned int* signal;
	size_t length;
} irPacket;


/**
 Initialises the module 
 Returns IR_SUCCESS/IR_FAILURE accordingly 
*/
int irInitialise(void);


/**
 Given a command id, issues the command to the TV via infrared
 Returns IR_SUCCESS/IR_FAILURE accordingly 
*/
int irTransmit(const cpCommand command);


/**
 Performs clean up.  Releases resources, etc.
*/
void irFree(void);


