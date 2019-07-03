#include "../../core_include/api.h"
#include "../../core_include/msg.h"
#include "../../core_include/audio.h"
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

typedef void(*ANDROID_PLAY_WAV)(const char* fileName);
ANDROID_PLAY_WAV gAndroidPlayWav;

typedef struct
{
	AUDIO_TYPE type;
}AUDIO_REQUEST;

static c_fifo s_request_fifo;

static void* render_thread(void* param)
{
	while (true)
	{
		AUDIO_REQUEST request;
		s_request_fifo.read(&request, sizeof(request));
		
		if (AUDIO_MAX <= request.type)
		{
			continue;
		}
		if(gAndroidPlayWav)
		{
			gAndroidPlayWav("heart_beat.wav");
		}
	}
}

void c_audio::init()
{
	static bool s_flag = false;
	if (s_flag)
	{
		return;
	}

	unsigned long pid;
	create_thread(&pid, 0, render_thread, 0);
	s_flag = true;
}

int c_audio::play(AUDIO_TYPE type)
{
	if (AUDIO_MAX <= type)
	{
		return -1;
	}

	init();

	AUDIO_REQUEST request;
	request.type = type;
	s_request_fifo.write(&request, sizeof(request));
	return 0;
}
