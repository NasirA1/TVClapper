/* Use the newer ALSA API */
#define ALSA_PCM_NEW_HW_PARAMS_API

#include <stdlib.h>
#include <alsa/asoundlib.h>
#include <pthread.h>
#include "settings.h"
#include "audiorecorder.h"



pthread_t thr;
pthread_mutex_t mutex;
snd_pcm_t *handle;
snd_pcm_uframes_t frames;
unsigned char* buffer;
BOOL running;
size_t buffersize;


BOOL arIsRunning(void)
{
	return running;
}


void arAcquireBuffer(void)
{
	//printf("Acquired buffer\n");
	pthread_mutex_lock(&mutex);
}


void arReleaseBuffer(void)
{
	//printf("Released buffer\n");
	pthread_mutex_unlock(&mutex);
}


const unsigned char* arBuffer(void)
{
	return buffer;
}


const size_t arBufferSize(void)
{
	return buffersize;
}


void* entry_point(void *arg)
{
	int rc;

	fprintf(stderr, "Now listening for commands...\n");

  while (running)
	{
		arAcquireBuffer();
    rc = snd_pcm_readi(handle, buffer, frames);
	//stream to stdout - useful for testing/debugging
	//write(1, buffer, buffersize);
		arReleaseBuffer();

    if (rc == -EPIPE) {
      /* EPIPE means overrun */
      fprintf(stderr, "overrun occurred\n");
      snd_pcm_prepare(handle);
    }
		else if (rc < 0) {
      fprintf(stderr, "error from read: %s\n", snd_strerror(rc));
			running = FALSE;
    }
		else if (rc != (int)frames) {
      fprintf(stderr, "short read, read %d frames\n", rc);
    }
 }

	return NULL;
}



int arInitialise(void)
{
  snd_pcm_hw_params_t *params;
  unsigned int val;
  int rc, dir;

	running = FALSE;

  /* Open PCM device for recording (capture). */
  rc = snd_pcm_open(&handle, RECORDER_DEVICE, SND_PCM_STREAM_CAPTURE, 0);
  if (rc < 0) {
    fprintf(stderr, "unable to open pcm device: %s\n", snd_strerror(rc));
    return rc;
  }
	else
	{
		fprintf(stderr, "Successfully opened default capture device.\n");
	}

  /* Allocate a hardware parameters object. */
  snd_pcm_hw_params_alloca(&params);

  /* Fill it in with default values. */
  snd_pcm_hw_params_any(handle, params);

  /* Set the desired hardware parameters. */

  /* Interleaved mode */
  snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);

  /* Signed 16-bit little-endian format */
  snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);
	fprintf(stderr, "Format set to PCM Signed 16bit Little Endian.\n");


  /* Channels */
  snd_pcm_hw_params_set_channels(handle, params, NUM_CHANNELS);
	fprintf(stderr, "Channels set to %d.\n", NUM_CHANNELS);


  /* sampling rate */
  val = SAMPLE_RATE;
  snd_pcm_hw_params_set_rate_near(handle, params, &val, &dir);
	fprintf(stderr, "Samplerate set to %d.\n", val);

  /* Set period to FRAMES_PER_BUFFER frames. */
  frames = FRAMES_PER_BUFFER;
  snd_pcm_hw_params_set_period_size_near(handle, params, &frames, &dir);

  /* Write the parameters to the driver */
  rc = snd_pcm_hw_params(handle, params);
  if (rc < 0) {
    fprintf(stderr, "unable to set hw parameters: %s\n", snd_strerror(rc));
    return rc;
  }

  /* Use a buffer large enough to hold one period */
  snd_pcm_hw_params_get_period_size(params, &frames, &dir);
  buffersize = frames * 2 * NUM_CHANNELS; /* 2 bytes/sample * channels */
  buffer = (unsigned char*) malloc(buffersize);

  /* We want to loop forever */
  //snd_pcm_hw_params_get_period_time(params, &val, &dir);

  return 0;
}


int arStartRecording(void)
{
	if(running) return 1;

  if(pthread_mutex_init(&mutex, NULL))
  {
    printf("Unable to initialize mutex\n");
    return -1;
  }

  if(pthread_create(&thr, NULL, &entry_point, NULL))
  {
  	fprintf(stderr, "Could not create recorder thread!\n");
		running = FALSE;
    return -1;
  }

	running = TRUE;
	return 0;
}


void arStopRecording(void)
{
	running = FALSE;
}


void arFree(void)
{
	running = FALSE;
	sleep(500);
  snd_pcm_drain(handle);
  snd_pcm_close(handle);
  pthread_mutex_destroy(&mutex);
  free(buffer);
}


