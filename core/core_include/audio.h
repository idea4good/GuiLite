#ifndef __AUDIO_MANGE_H__
#define __AUDIO_MANGE_H__

enum AUDIO_TYPE
{
	AUDIO_HEART_BEAT,
	AUDIO_ALARM,
	AUDIO_MAX
};

class c_audio
{
public:
	static int play(AUDIO_TYPE type);
private:
	static void init();
};

#endif
