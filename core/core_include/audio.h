#ifndef AUDIO_MANGE_H
#define AUDIO_MANGE_H

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
