/**************************************************************

  Module:		Command-parser
  Version:	2013.04.18
  Author:		Nasir Ahmad (na200@kent.ac.uk)

  Description
  Command-parser operates above the 'clap-detector' module.
  Whilst the 'clap-detector' is responsible for detecting a
  single clap, the command-parser's job is to group detected
  claps together in order to recognise known clap sequences
  which represent different TV remote-control commands.

**************************************************************/


#pragma once
#include <stdint.h>
#include "utils.h"

//Number of commands supported
#define COMMAND_COUNT	(6)

//Size of the command buffer
//Currently 5 slots, each representing a time-window of 200ms
//A 'command' is therefore 1-second long
#define COMMAND_BUFFER_SIZE	(5)

//Size of clap-patterns for each supported command
#define COMMAND_PATTERN_COUNT	(2)


/**
 Supported commands
*/
typedef enum tagcpCommand
{
	cpCmdUnknown,
	cpCmdPower,
	cpCmdVolUp,
	cpCmdVolDown,
	cpCmdChanUp,
	cpCmdChanDown
} cpCommand;


/**
 Command info. Id and it's string representation
 Useful for display purposes (testing/debugging)
*/
typedef struct tagcpCommandInfo {
    int id;
    char* name;
} cpCommandInfo;


/**
 A tuple (key/value pair) comprising of command id
 and it's pair of clap-patterns
*/
typedef struct tagcpCommandPattern
{
	cpCommand commandId;
	BOOL patterns[COMMAND_PATTERN_COUNT][COMMAND_BUFFER_SIZE];
} cpCommandPattern;


/**
 Initialises the module
*/
void cpInitialise(void);


/**
 Signals to the command-parser the detection of a clap
 Registers a single clap
*/
void cpSignal(int64_t elapsedSinceLastSignal);


/**
 Determines if a command has been recongised so far.
 Returns either a recognised command or cpCmdUnknown (If no
 command was recognised)
*/
cpCommand cpRecogniseCommand(void);


/**
 Returns the 'screen-friendly' name of the given integer identifier
 (e.g. a command id).  Useful for display purposes (testing/debugging) 
*/
char* cpId2str(const int id);


