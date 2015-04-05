/**************************************************************

  Module:		Settings
  Version:	2013.04.18
  Author:		Nasir Ahmad (na200@kent.ac.uk)

  Description
	Contains compile-time program settings

**************************************************************/


#pragma once

#ifdef _PC /* PC environment settings*/

#define SAMPLE_RATE  (44100)
#define FRAMES_PER_BUFFER (1024)
#define RECORDER_DEVICE	"default"
#define SCALE_FACTOR	1.000000
#define CD_SENSITIVITY	20.000000

#else /* Raspberry Pi environment settings */

#define SAMPLE_RATE  (24000)
#define FRAMES_PER_BUFFER (512)
#define RECORDER_DEVICE	"hw:1"
#define SCALE_FACTOR	10.000000
#define CD_SENSITIVITY	1.200000

#endif


#define NUM_CHANNELS    (1) /* DO NOT CHANGE THIS! */
#define CD_MINGAPMS			(50)


