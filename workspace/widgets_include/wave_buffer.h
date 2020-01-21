#ifndef GUILITE_WIDGETS_INCLUDE_WAVE_BUFFER_H
#define GUILITE_WIDGETS_INCLUDE_WAVE_BUFFER_H

#include "../core_include/api.h"
#include <string.h>
#include <stdio.h>

#define WAVE_BUFFER_LEN	1024
#define WAVE_READ_CACHE_LEN	8
#define BUFFER_EMPTY	-1111
#define BUFFER_FULL		-2222;
#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))

class c_wave_buffer
{
public:
	c_wave_buffer()
	{
		m_head = m_tail = m_min_old = m_max_old =
			m_min_older = m_max_older = m_last_data = m_read_cache_sum = m_refresh_sequence = 0;
		memset(m_wave_buf, 0, sizeof(m_wave_buf));
		memset(m_read_cache_min, 0, sizeof(m_read_cache_min));
		memset(m_read_cache_mid, 0, sizeof(m_read_cache_mid));
		memset(m_read_cache_max, 0, sizeof(m_read_cache_max));
	}
	int write_wave_data(short data)
	{
		if ((m_tail + 1) % WAVE_BUFFER_LEN == m_head)
		{//full
			//log_out("wave buf full\n");
			return BUFFER_FULL;
		}
		m_wave_buf[m_tail] = data;
		m_tail = (m_tail + 1) % WAVE_BUFFER_LEN;
		return 1;
	}
	int read_wave_data_by_frame(short &max, short &min, short frame_len, unsigned int sequence, short offset)
	{
		if (m_refresh_sequence != sequence)
		{
			m_refresh_sequence = sequence;
			m_read_cache_sum = 0;
		}
		else if (offset < m_read_cache_sum)//(m_refresh_sequence == sequence && offset < m_fb_sum)
		{
			max = m_read_cache_max[offset];
			min = m_read_cache_min[offset];
			return m_read_cache_mid[offset];
		}

		m_read_cache_sum++;
		ASSERT(m_read_cache_sum <= WAVE_READ_CACHE_LEN);
		int i, data;
		int tmp_min = m_last_data;
		int tmp_max = m_last_data;
		int mid = (m_min_old + m_max_old) >> 1;

		i = 0;
		while (i++ < frame_len)
		{
			data = read_data();
			if (BUFFER_EMPTY == data)
			{
				break;
			}
			m_last_data = data;

			if (data < tmp_min) { tmp_min = data; }
			if (data > tmp_max) { tmp_max = data; }
		}

		min = m_read_cache_min[offset] = MIN(m_min_old, MIN(tmp_min, m_min_older));
		max = m_read_cache_max[offset] = MAX(m_max_old, MAX(tmp_max, m_max_older));

		m_min_older = m_min_old;
		m_max_older = m_max_old;
		m_min_old = tmp_min;
		m_max_old = tmp_max;
		return (m_read_cache_mid[offset] = mid);
	}
	void reset()
	{
		m_head = m_tail;
	}

	void clear_data()
	{
		m_head = m_tail = 0;
		memset(m_wave_buf, 0, sizeof(m_wave_buf));
	}
	short get_cnt()
	{
		return (m_tail >= m_head) ? (m_tail - m_head) : (m_tail - m_head + WAVE_BUFFER_LEN);
	}
private:
	int read_data()
	{
		if (m_head == m_tail)
		{//empty
			//log_out("wave buf empty\n");
			return BUFFER_EMPTY;
		}
		int ret = m_wave_buf[m_head];
		m_head = (m_head + 1) % WAVE_BUFFER_LEN;
		return ret;
	}
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
