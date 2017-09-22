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
	int play(AUDIO_TYPE type);
	static c_audio* get_instance()
	{
		static c_audio s_the_audio;
		return &s_the_audio;
	}
private:
	void init();
};

#endif
