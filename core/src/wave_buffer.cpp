#include "../core_include/api.h"
#include "../core_include/wave_buffer.h"
#include <string.h>
#include <stdio.h>

#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))

c_wave_buffer::c_wave_buffer()
{
	m_head = m_tail = m_min_old = m_max_old =
	m_min_older = m_max_older = m_last_data =
	m_refresh_sqence = 0;

	memset(m_wave_buf, 0, sizeof(m_wave_buf));
	memset(m_fb_min, 0, sizeof(m_fb_min));
	memset(m_fb_mid, 0, sizeof(m_fb_mid));
	memset(m_fb_max, 0, sizeof(m_fb_max));
}

short c_wave_buffer::get_cnt()
{
	return (m_tail >= m_head)?(m_tail - m_head):(m_tail - m_head + WAVE_BUFFER_LEN);
}

int c_wave_buffer::write_wave_data(short data)
{
	if ((m_tail + 1) % WAVE_BUFFER_LEN == m_head)
	{//full
		//printf("full\n");
		//fflush(stdout);
		return BUFFER_FULL;
	}
	m_wave_buf[m_tail] = data;
	m_tail = (m_tail + 1) % WAVE_BUFFER_LEN;
	return 1;
}

int c_wave_buffer::read_data()
{
	if (m_head == m_tail)
	{//empty
		//printf("empty\n");
		//fflush(stdout);
		return BUFFER_EMPTY;
	}
	int ret = m_wave_buf[m_head];
	m_head = (m_head + 1) % WAVE_BUFFER_LEN;
	return ret;
}

int c_wave_buffer::read_wave_data_by_frame(short &max, short &min, short len, unsigned int seq)
{
	unsigned int offset = (seq >> 8) & 0xff;
	if(offset > WAVE_FRAME_LEN)
	{
		ASSERT(FALSE);
	}

	if(((m_refresh_sqence & 0xff) == (seq & 0xff)) &&
			((m_refresh_sqence & 0xffff0000) != (seq & 0xffff0000)))
	{
		max = m_fb_max[offset];
		min = m_fb_min[offset];
		return m_fb_mid[offset];
	}

	m_refresh_sqence = seq;

	int i, data;
	int tmp_min = m_last_data;
	int tmp_max = m_last_data;
	int mid = (m_min_old + m_max_old)>>1;

	i = 0;
	while(i++ < len)
	{
		data = read_data();
		if(BUFFER_EMPTY == data)
		{
			break;
		}
		m_last_data = data;

		if(data < tmp_min){tmp_min = data;}
		if(data > tmp_max){tmp_max = data;}
	}

	min = m_fb_min[offset] = MIN(m_min_old, MIN(tmp_min, m_min_older));
	max = m_fb_max[offset] = MAX(m_max_old, MAX(tmp_max, m_max_older));

	m_min_older = m_min_old;
	m_max_older = m_max_old;
	m_min_old = tmp_min;
	m_max_old = tmp_max;
	return (m_fb_mid[offset] = mid);
}

void c_wave_buffer::clear_data()
{
	m_head = m_tail = 0;
	memset(m_wave_buf, 0, sizeof(m_wave_buf));
}

void c_wave_buffer::reset()
{
	m_head = m_tail;
}
