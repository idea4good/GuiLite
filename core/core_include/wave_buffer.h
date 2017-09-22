#ifndef _XT_GUI_WAVE_H_
#define _XT_GUI_WAVE_H_

#define WAVE_BUFFER_LEN	64
#define WAVE_FRAME_LEN	8
#define BUFFER_EMPTY	-1111
#define BUFFER_FULL		-2222;
class c_wave_buffer
{
public:
	c_wave_buffer();
	int write_wave_data(short data);
	int read_wave_data_by_frame(short &max, short &min, short len, unsigned int seq);
	void reset();
	void clear_data();
	short get_cnt();
private:
	int read_data();
	short m_wave_buf[WAVE_BUFFER_LEN];
	short m_head;
	short m_tail;

	int m_min_old;
	int m_max_old;
	int m_min_older;
	int m_max_older;
	int m_last_data;

	short 	m_fb_min[WAVE_FRAME_LEN];
	short 	m_fb_mid[WAVE_FRAME_LEN];
	short 	m_fb_max[WAVE_FRAME_LEN];
	unsigned int m_refresh_sqence;
};

#endif
