#ifndef WAVE_BUFFER_H
#define WAVE_BUFFER_H

#define WAVE_BUFFER_LEN	1024
#define WAVE_READ_CACHE_LEN	8
#define BUFFER_EMPTY	-1111
#define BUFFER_FULL		-2222;
class c_wave_buffer
{
public:
	c_wave_buffer();
	int write_wave_data(short data);
	int read_wave_data_by_frame(short &max, short &min, short frame_len, unsigned int sequence, short offset);
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

	short 	m_read_cache_min[WAVE_READ_CACHE_LEN];
	short 	m_read_cache_mid[WAVE_READ_CACHE_LEN];
	short 	m_read_cache_max[WAVE_READ_CACHE_LEN];
	short	m_read_cache_sum;
	unsigned int m_refresh_sequence;
};

#endif
