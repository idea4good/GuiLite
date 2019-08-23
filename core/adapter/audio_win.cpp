#include <windows.h>
#include <Audioclient.h>
#include <mmdeviceapi.h>

#include "../../core_include/api.h"
#include "../../core_include/msg.h"
#include "../../core_include/audio.h"

#ifndef AUDCLNT_STREAMFLAGS_AUTOCONVERTPCM
	#define AUDCLNT_STREAMFLAGS_AUTOCONVERTPCM 0x80000000
#endif
#define AUDIO_CHANNELS_MONO     1
#define AUDIO_SAMPLE_RATE       44000
#define AUDIO_BITS              16
#define AUDIO_BLOCK_ALIGN       (AUDIO_CHANNELS_MONO * (AUDIO_BITS >> 3))
#define AUDIO_BYTE_RATE         (AUDIO_SAMPLE_RATE * AUDIO_BLOCK_ALIGN)
#define AUDIO_OUTPUT_BUF_LEN	(10000000 * 5)	//5 seconds long.

#define CHECK_ERROR(ret) if(ret != 0){ASSERT(false);}

typedef struct
{
	AUDIO_TYPE type;
}AUDIO_REQUEST;

typedef struct
{
	BYTE* p_data;
	int size;
}WAV_RESOURCE;

static WAV_RESOURCE s_wav_resource[AUDIO_MAX];
static c_fifo s_request_fifo;
static IAudioClient* s_audio_client;
static IAudioRenderClient* s_audio_render_client;
static HANDLE s_audio_event;

//Should be call by UWP, and UWP create audio client.
void set_audio_client(IAudioClient* audio_client)
{
	s_audio_client = audio_client;
}

static WAVEFORMATEX s_wav_format = {
	WAVE_FORMAT_PCM,
	AUDIO_CHANNELS_MONO,
	AUDIO_SAMPLE_RATE,
	AUDIO_BYTE_RATE,
	AUDIO_BLOCK_ALIGN,
	AUDIO_BITS,
	0
};

static int register_wav_resouce(AUDIO_TYPE type, const wchar_t* wav_path)
{
	if (s_wav_resource[type].p_data)
	{
		return 0;
	}
  
	void* hFile = CreateFile(wav_path, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		log_out("Open wave file failed\n");
		return -1;
	}

	LARGE_INTEGER ret;
	GetFileSizeEx(hFile, &ret);
	int size = ret.LowPart;

	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0x2C, 0, FILE_BEGIN))
	{
		ASSERT(false);
		return -2;
	}
	size -= 0x2C;

	BYTE* p_data = (BYTE*)malloc(size);
	DWORD read_num;
	ReadFile(hFile, p_data, size, &read_num, 0);

	s_wav_resource[type].p_data = p_data;
	s_wav_resource[type].size = size;
	return 0;
}

static int load_wav_chunk(BYTE* p_des, int des_size, BYTE* p_src, int src_size)
{
	if (des_size <= 0 || src_size <= 0)
	{
		return -1;
	}

	int write_size = (src_size > des_size) ? des_size : src_size;
	memcpy(p_des, p_src, write_size);
	memset(p_des + write_size, 0, (des_size - write_size));
	return write_size;
}

static int play_wav(BYTE* p_data, int size)
{
	if (0 == p_data || 0 >= size)
	{
		return -1;
	}

	UINT32 bufferFrameCount;
	UINT32 numFramesAvailable;
	UINT32 numFramesPadding;
	BYTE* p_buffer = 0;
	int ret = s_audio_client->GetBufferSize(&bufferFrameCount);
	CHECK_ERROR(ret);
	
	int offset = 0;
	while (WaitForSingleObject(s_audio_event, INFINITE) == WAIT_OBJECT_0)
	{
		ret = s_audio_client->GetCurrentPadding(&numFramesPadding);
		CHECK_ERROR(ret);

		numFramesAvailable = bufferFrameCount - numFramesPadding;
		if (numFramesAvailable < 1600)
		{
			Sleep(10);
			continue;
		}

		ret = s_audio_render_client->GetBuffer(numFramesAvailable, &p_buffer);
		CHECK_ERROR(ret);

		ret = load_wav_chunk(p_buffer, numFramesAvailable * s_wav_format.nBlockAlign, p_data + offset, (size - offset));

		if (ret > 0)
		{
			s_audio_render_client->ReleaseBuffer((ret / s_wav_format.nBlockAlign), 0);
			offset += ret;
		}
		else
		{
			s_audio_render_client->ReleaseBuffer(0, AUDCLNT_BUFFERFLAGS_SILENT);
			break;
		}
	}	
	return 0;
}

static void* render_thread(void* param)
{
	s_audio_client->Start();
	while (true)
	{
		AUDIO_REQUEST request;
		s_request_fifo.read(&request, sizeof(request));
		
		if (AUDIO_MAX <= request.type)
		{
			ASSERT(false);
			continue;
		}
		play_wav(s_wav_resource[request.type].p_data, s_wav_resource[request.type].size);
	}
	s_audio_client->Stop();
}

static int init_audio_client()
{
	if (s_audio_client)
	{
		return 0;
	}

	//For desktop only, could not pass Windows Store certification.
	/*
	int ret = CoInitializeEx(0, COINIT_MULTITHREADED);
	CHECK_ERROR(ret);

	IMMDeviceEnumerator *pEnumerator = nullptr;
	ret = CoCreateInstance(__uuidof(MMDeviceEnumerator), 0,
	CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),
	(void**)&pEnumerator);
	CHECK_ERROR(ret);

	IMMDevice* audio_output_device;
	pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &audio_output_device);
	if (0 == audio_output_device)
	{
	ASSERT(false);
	}

	ret = audio_output_device->Activate(__uuidof(IAudioClient), CLSCTX_ALL, 0, (void**)&s_audio_client);
	CHECK_ERROR(ret);
	return 0;
	*/
	return -1;
}

void c_audio::init()
{
	static bool s_flag = false;
	if (s_flag)
	{
		return;
	}

	register_wav_resouce(AUDIO_HEART_BEAT, L"heart_beat.wav");
	
	if (0 > init_audio_client())
	{
		return;
	}

	int ret = s_audio_client->Initialize(AUDCLNT_SHAREMODE_SHARED,
									AUDCLNT_STREAMFLAGS_AUTOCONVERTPCM | AUDCLNT_STREAMFLAGS_EVENTCALLBACK,
									AUDIO_OUTPUT_BUF_LEN * 2, 0, &s_wav_format,	0);
	CHECK_ERROR(ret);

	s_audio_event = CreateEventEx(0, 0, 0, EVENT_ALL_ACCESS);
	ret = s_audio_client->SetEventHandle(s_audio_event);
	CHECK_ERROR(ret);

	ret = s_audio_client->GetService(__uuidof(IAudioRenderClient), (void**)&s_audio_render_client);
	CHECK_ERROR(ret);

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
	if (!s_audio_client || !s_audio_render_client)
	{
		return -2;
	}

	AUDIO_REQUEST request;
	request.type = type;
	s_request_fifo.write(&request, sizeof(request));
	return 0;
}
