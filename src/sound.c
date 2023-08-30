#include <math.h>
#include <pthread.h>
#include "../../printer/src/Canvas.h"
#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>


void Beep(double freq, int duration){
    snd_pcm_t *pcm;
    snd_pcm_open(&pcm, "default", SND_PCM_STREAM_PLAYBACK, 0);
    snd_pcm_set_params(pcm, SND_PCM_FORMAT_S16_LE, SND_PCM_ACCESS_RW_INTERLEAVED, 1, 48000, 1, 50000);

    short buf[48000 * duration / 1000];
    for (int i = 0; i < sizeof(buf)/sizeof(buf[0]); i++) {
        buf[i] = 32767 * sin(2 * M_PI * freq * i / 48000.0);
    }

    snd_pcm_writei(pcm, buf, sizeof(buf)/sizeof(buf[0]));
    snd_pcm_close(pcm);

}

typedef struct beepArgs{
	double freq;
	int duration;
}BeepArgs;

void *beep(void *vargp){

    BeepArgs *args = vargp;

    Beep(args->freq,args->duration);

    return NULL;
}

void BeepThread(double freq, int duration){

    // BeepArgs *args = malloc(sizeof(BeepArgs));
    // args->freq = freq;
    // args->duration = duration;
    //
    // pthread_t thread_id;
    // pthread_create(&thread_id, NULL, beep, args);
    // //pthread_join(thread_id, NULL);
    // free(args);
	
}

void note(char _c, int duration, double octave)
{
	switch (_c)
	{
	case 'c':
		Beep(octave *  261.63, duration);
		break;
	case 'd':
		Beep(octave * 293.66, duration);
		break;
	case 'e':
		Beep(octave * 329.63, duration);
		break;
	case 'f':
		Beep(octave * 349.23, duration);
		break;
	case 'g':
		Beep(octave * 392.00, duration);
		break;
	case 'a':
		Beep(octave * 440.00, duration);
		break;
	case 'b':
		Beep(octave * 493.88, duration);
		break;
	case 'C':
		Beep(octave * 523.25, duration);
		break;
	case 'D':
		Beep(octave * 587.33, duration);
		break;
	case 'E':
		Beep(octave * 659.25, duration);
		break;
	case 'F':
		Beep(octave * 698.46, duration);
		break;
	case 'G':
		Beep(octave * 783.99, duration);
		break;
	case 'A':
		Beep(octave * 880.00, duration);
		break;
	case '1':
		Beep(octave * 1046.50, duration);
		break;
	}
}

int playMusic(){
	double oct = 1;
	goto one;
	// ##############
	one:
	note('E', 400, oct);

	note('b', 200, oct);
	note('C', 200, oct);

	note('D', 400, oct);

	note('C', 200, oct);
	note('b', 200, oct);
	// ##############
	two:
	note('a', 400, oct);

	note('a', 200, oct);
	note('C', 200, oct);

	note('E', 400, oct);

	note('D', 200, oct);
	note('C', 200, oct);
	// ##############
	three:
	note('b', 400, oct);

	note('b', 200, oct);
	note('C', 200, oct);

	note('D', 400, oct);

	note('E', 400, oct);
	// ##############
	four:

	note('C', 400, oct);

	note('a', 400, oct);

	note('a', 400, oct);
	// ##############
	printer_msleep(400);
	// ##############
	five:
	note('D', 400, oct);

	note('F', 200, oct);

	note('A', 400, oct);

	note('G', 200, oct);
	note('F', 200, oct);
	// ##############
	six:
	note('E', 600, oct);

	note('C', 200, oct);
	
	note('E', 400, oct);

	note('D', 200, oct);
	note('C', 200, oct);
	// ##############
	seven:
	note('b', 400, oct);

	note('b', 200, oct);
	note('C', 200, oct);

	note('D', 400, oct);

	note('E', 400, oct);
	// ##############
	eight:
	note('C', 400, oct);

	note('a', 400, oct);

	note('a', 400, oct);
	// ##############
	printer_msleep(400);
	// ##############
	//goto one;

	return 0;
}
